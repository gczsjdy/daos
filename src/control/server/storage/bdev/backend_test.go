//
// (C) Copyright 2018-2019 Intel Corporation.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
// The Government's rights to use, modify, reproduce, release, perform, display,
// or disclose this software are subject to the terms of the Apache License as
// provided in Contract No. 8F-30005.
// Any reproduction of computer software, computer software documentation, or
// portions thereof marked with this legend must also reproduce the markings.
//
package bdev

import (
	"fmt"
	"testing"

	"github.com/google/go-cmp/cmp"
	"github.com/pkg/errors"

	"github.com/daos-stack/daos/src/control/common"
	"github.com/daos-stack/daos/src/control/lib/spdk"
	"github.com/daos-stack/daos/src/control/server/storage"
)

func concat(base string, idx int32) string {
	return fmt.Sprintf("%s-%d", base, idx)
}

func getIdx(varIdx ...int32) int32 {
	if len(varIdx) == 0 {
		varIdx = append(varIdx, 1)
	}

	return varIdx[0]
}

func mockSpdkController(varIdx ...int32) spdk.Controller {
	idx := getIdx(varIdx...)

	return spdk.Controller{
		Model:    concat("model", idx),
		Serial:   concat("serial", idx),
		PCIAddr:  concat("pciAddr", idx),
		FWRev:    concat("fwRev", idx),
		SocketID: idx,
	}
}

func mockSpdkNamespace(varIdx ...int32) spdk.Namespace {
	idx := getIdx(varIdx...)

	return spdk.Namespace{
		ID:           idx,
		Size:         idx,
		CtrlrPciAddr: concat("pciAddr", idx),
	}
}

func mockSpdkDeviceHealth(varIdx ...int32) spdk.DeviceHealth {
	idx := getIdx(varIdx...)

	return spdk.DeviceHealth{
		CtrlrPciAddr: concat("pciAddr", idx),
	}
}

func TestBdevBackendCoalesce(t *testing.T) {
	type input struct {
		spdkControllers  []spdk.Controller
		spdkNamespaces   []spdk.Namespace
		spdkDeviceHealth []spdk.DeviceHealth
	}

	scEmptyNamespaces := storage.MockNvmeController()
	scEmptyNamespaces.Namespaces = nil
	scEmptyNamespaces.Health = nil
	scEmptyDeviceHealth := storage.MockNvmeController()
	scEmptyDeviceHealth.Health = nil

	for name, tc := range map[string]struct {
		input  input
		expScs []*storage.NvmeController
		expErr error
	}{
		"empty input": {
			input:  input{},
			expScs: storage.NvmeControllers{},
		},
		"empty namespaces": {
			input: input{
				spdkControllers: []spdk.Controller{mockSpdkController()},
			},
			expScs: storage.NvmeControllers{scEmptyNamespaces},
		},
		"empty deviceHealth": {
			input: input{
				spdkControllers: []spdk.Controller{mockSpdkController()},
				spdkNamespaces:  []spdk.Namespace{mockSpdkNamespace()},
			},
			expScs: storage.NvmeControllers{scEmptyDeviceHealth},
		},
		"single deviceHealth": {
			input: input{
				spdkControllers:  []spdk.Controller{mockSpdkController()},
				spdkNamespaces:   []spdk.Namespace{mockSpdkNamespace()},
				spdkDeviceHealth: []spdk.DeviceHealth{mockSpdkDeviceHealth()},
			},
			expScs: storage.NvmeControllers{storage.MockNvmeController()},
		},
		"extra namespace/deviceHealth": {
			input: input{
				spdkControllers:  []spdk.Controller{mockSpdkController()},
				spdkNamespaces:   []spdk.Namespace{mockSpdkNamespace(), mockSpdkNamespace(2)},
				spdkDeviceHealth: []spdk.DeviceHealth{mockSpdkDeviceHealth(), mockSpdkDeviceHealth(2)},
			},
			expScs: storage.NvmeControllers{storage.MockNvmeController()},
		},
		"two controllers": {
			input: input{
				spdkControllers:  []spdk.Controller{mockSpdkController(), mockSpdkController(2)},
				spdkNamespaces:   []spdk.Namespace{mockSpdkNamespace(), mockSpdkNamespace(2)},
				spdkDeviceHealth: []spdk.DeviceHealth{mockSpdkDeviceHealth(), mockSpdkDeviceHealth(2)},
			},
			expScs: storage.NvmeControllers{storage.MockNvmeController(), storage.MockNvmeController(2)},
		},
		"duplicate deviceHealth": {
			input: input{
				spdkControllers:  []spdk.Controller{mockSpdkController()},
				spdkDeviceHealth: []spdk.DeviceHealth{mockSpdkDeviceHealth(), mockSpdkDeviceHealth()},
			},
			expErr: errors.New("duplicate"),
		},
	} {
		t.Run(name, func(t *testing.T) {
			gotScs, gotErr := coalesceControllers(tc.input.spdkControllers,
				tc.input.spdkNamespaces, tc.input.spdkDeviceHealth)

			common.CmpErr(t, tc.expErr, gotErr)
			if gotErr != nil {
				return
			}
			if diff := cmp.Diff(tc.expScs, gotScs); diff != "" {
				t.Fatalf("\nunexpected output (-want, +got):\n%s\n", diff)
			}
		})
	}
}

func TestBdevBackendGetFormattedController(t *testing.T) {
	type input struct {
		pciAddr         string
		spdkControllers []spdk.Controller
		spdkNamespaces  []spdk.Namespace
	}

	// We don't get device health back after a format, so the expected
	// controller results shouldn't include it.
	scEmptyNamespaces := storage.MockNvmeController()
	scEmptyNamespaces.Namespaces = nil
	scEmptyNamespaces.Health = nil
	scNormalNoHealth := storage.MockNvmeController()
	scNormalNoHealth.Health = nil

	for name, tc := range map[string]struct {
		input  input
		expSc  *storage.NvmeController
		expErr error
	}{
		"empty input": {
			input:  input{},
			expErr: errors.New("unable to resolve"),
		},
		"wrong pciAddr": {
			input: input{
				pciAddr:         "abc123",
				spdkControllers: []spdk.Controller{mockSpdkController()},
				spdkNamespaces:  []spdk.Namespace{mockSpdkNamespace()},
			},
			expErr: errors.New("unable to resolve"),
		},
		"empty namespaces": {
			input: input{
				pciAddr:         mockSpdkController().PCIAddr,
				spdkControllers: []spdk.Controller{mockSpdkController()},
			},
			expSc: scEmptyNamespaces,
		},
		"extra namespace": {
			input: input{
				pciAddr:         mockSpdkController().PCIAddr,
				spdkControllers: []spdk.Controller{mockSpdkController()},
				spdkNamespaces:  []spdk.Namespace{mockSpdkNamespace(), mockSpdkNamespace(2)},
			},
			expSc: scNormalNoHealth,
		},
		"two controllers": {
			input: input{
				pciAddr:         mockSpdkController().PCIAddr,
				spdkControllers: []spdk.Controller{mockSpdkController(), mockSpdkController(2)},
				spdkNamespaces:  []spdk.Namespace{mockSpdkNamespace(), mockSpdkNamespace(2)},
			},
			expSc: scNormalNoHealth,
		},
	} {
		t.Run(name, func(t *testing.T) {
			gotSc, gotErr := getFormattedController(tc.input.pciAddr, tc.input.spdkControllers,
				tc.input.spdkNamespaces)

			common.CmpErr(t, tc.expErr, gotErr)
			if gotErr != nil {
				return
			}
			if diff := cmp.Diff(tc.expSc, gotSc); diff != "" {
				t.Fatalf("\nunexpected output (-want, +got):\n%s\n", diff)
			}
		})
	}
}
