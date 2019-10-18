#!/usr/bin/python
"""
  (C) Copyright 2019 Intel Corporation.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
  The Government's rights to use, modify, reproduce, release, perform, display,
  or disclose this software are subject to the terms of the Apache License as
  provided in Contract No. B609815.
  Any reproduction of computer software, computer software documentation, or
  portions thereof marked with this legend must also reproduce the markings.
"""
from __future__ import print_function

import os
import avocado

from pydaos.raw import DaosPool, DaosApiError
from ior_test_base import IorTestBase

class NvmeIoVerification(IorTestBase):
    """Test class for NVMe with IO tests.

    Test Class Description:
        Test IO on nvme with different pool sizes and different data size.

    :avocado: recursive
    """

    @avocado.fail_on(DaosApiError)
    def test_nvme_io_verification(self):
        """Jira ID: DAOS-2649.

        Test Description:
            Test will run IOR with non standard transfer sizes for different
            set of pool sizes. Purpose is to verify io transaction to scm and
            nvme for different pool sizes under different situations.

        Use Cases:
            (1) Running IOR with different set of transfer size where first
            transfer size is < 4096 and then > 4096. Verify that data goes to
            scm if transfer size < 4096 and then it goes to nvme if transfer
            size is > 4096.
            (2) Repeat the case(1) with maximum nvme pool size that can be
            created.
            (3) Running IOR with different set of transfer size where the
            transfer size is > 4096 throughout. Verify that data goes to nvme
            as transfer size is > 4096.
            (4) Repeat the case(3) with maximum nvme pool size that can be
            created.
        :avocado: tags=all,daosio,full_regression,hw,nvme_io_verification
        """
        # Pool params
        pool_mode = self.params.get("mode", '/run/pool/createmode/*')
        pool_uid = os.geteuid()
        pool_gid = os.getegid()
        pool_group = self.params.get("setname", '/run/pool/createset/*')
        pool_svcn = self.params.get("svcn", '/run/pool/createsvc/')

        # Test params
        tests = self.params.get("ior_sequence", '/run/ior/*')
        transfer_size = self.params.get("tsize", '/run/ior/transfersize/*/')
        block_size = self.ior_cmd.block_size.value

        # Loop for every IOR object type
        for ior_param in tests:
            # Create and connect to a pool
            self.pool = DaosPool(self.context)
            self.pool.create(
                pool_mode, pool_uid, pool_gid, ior_param[0], pool_group,
                svcn=pool_svcn, nvme_size=ior_param[1])

            self.pool.connect(1 << 1)

            for tsize in transfer_size:
                # Get the current pool sizes
                size_before_ior = self.pool.pool_query()

                # Run ior with the parameters specified for this pass
                self.ior_cmd.transfer_size.update(tsize)
                # if transfer size is less thank 1K
                # update block size to 32K to keep it small
                if tsize <= 1000:
                    self.ior_cmd.block_size.update(32000)
                else:
                    self.ior_cmd.block_size.update(block_size)
                self.ior_cmd.set_daos_params(self.server_group, self.pool)
                self.run_ior(self.get_job_manager_command())

                # Verify IOR consumed the expected amount ofrom the pool
                self.verify_pool_size(size_before_ior, self.processes)

            try:
                if self.pool:
                    self.pool.disconnect()
                    self.pool.destroy(1)
                    self.pool = None
            except DaosApiError as error:
                self.log.error(
                    "Pool disconnect/destroy error: %s", str(error))
                self.fail("Failed to Destroy/Disconnect the Pool")
