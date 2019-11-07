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

package main

import (
	"fmt"
	"testing"

	"github.com/google/go-cmp/cmp"
	"github.com/pkg/errors"

	. "github.com/daos-stack/daos/src/control/client"
	"github.com/daos-stack/daos/src/control/common"
	ctlpb "github.com/daos-stack/daos/src/control/common/proto/ctl"
	"github.com/daos-stack/daos/src/control/logging"
)

func TestHasConnection(t *testing.T) {
	for name, tc := range map[string]struct {
		results     ResultMap
		runCmdOut   string
		lookPathErr error
		out         string
		expCmds     []string
	}{
		"no connections": {
			results: ResultMap{},
			out:     "No active connections!\n",
		},
		"single successful connection": {
			results: ResultMap{"1.2.3.4:10000": ClientResult{"1.2.3.4:10000", nil, nil}},
			out:     "Active: 1.2.3.4:10000\n",
		},
		"single failed connection": {
			results: ResultMap{"1.2.3.4:10000": ClientResult{"1.2.3.4:10000", nil, MockErr}},
			out:     "Inactive: 1.2.3.4:10000\nNo active connections!\n",
		},
		"multiple successful connections": {
			results: ResultMap{
				"1.2.3.4:10000": ClientResult{"1.2.3.4:10000", nil, nil},
				"1.2.3.5:10001": ClientResult{"1.2.3.5:10001", nil, nil},
			},
			out: "Active: 1.2.3.4:10000,1.2.3.5:10001\n",
		},
		"multiple failed connections": {
			results: ResultMap{"1.2.3.4:10000": ClientResult{"1.2.3.4:10000", nil, MockErr}, "1.2.3.5:10001": ClientResult{"1.2.3.5:10001", nil, MockErr}},
			out:     "Inactive: 1.2.3.4:10000,1.2.3.5:10001\nNo active connections!\n",
		},
		"failed and successful connections": {
			results: ResultMap{"1.2.3.4:10000": ClientResult{"1.2.3.4:10000", nil, MockErr}, "1.2.3.5:10001": ClientResult{"1.2.3.5:10001", nil, nil}},
			out:     "Inactive: 1.2.3.4:10000\nActive: 1.2.3.5:10001\n",
		},
		"multiple successful connections with nodeset fold": {
			runCmdOut: "1.2.3.[4,5]:10001",
			results: ResultMap{
				"1.2.3.4:10001": ClientResult{"1.2.3.4:10001", nil, nil},
				"1.2.3.5:10001": ClientResult{"1.2.3.5:10001", nil, nil},
			},
			out:     "Active: 1.2.3.[4,5]:10001\n",
			expCmds: []string{"nodeset -f --separator=',' 1.2.3.4:10001,1.2.3.5:10001"},
		},
	} {
		t.Run(name, func(t *testing.T) {
			log, buf := logging.NewTestLogger(t.Name())
			defer common.ShowBufferOnFailure(t, buf)

			commands := []string{}
			if tc.expCmds == nil {
				tc.expCmds = []string{}
			}
			// if nothing useful from runCmd, trigger fallback
			if tc.runCmdOut == "" {
				tc.lookPathErr = errors.New("no binary")
			}

			mockLookPath := func(string) (string, error) {
				fmt.Printf("lp: %#v\n", tc.lookPathErr)
				return "", tc.lookPathErr
			}

			mockRunFn := func(in string) (string, error) {
				commands = append(commands, in)
				fmt.Printf("mr: %#v\n", tc.runCmdOut)
				return tc.runCmdOut, nil
			}

			runner := &cmdRunner{log: log, runCmd: mockRunFn, lookPath: mockLookPath}
			_, out := runner.hasConns(tc.results)
			if diff := cmp.Diff(tc.out, out); diff != "" {
				t.Fatalf("unexpected output (-want, +got):\n%s\n", diff)
			}

			if diff := cmp.Diff(tc.expCmds, commands); diff != "" {
				t.Fatalf("unexpected command (-want, +got):\n%s\n", diff)
			}
		})
	}
}

func TestCheckSprint(t *testing.T) {
	for name, tt := range map[string]struct {
		m   string
		out string
	}{
		"nvme scan summary without health": {
			fmt.Sprint(MockScanResp(MockCtrlrs, nil, nil, MockServers, true)),
			"1.2.3.4:10000\n\tSummary:\n\t\tSCM: 0.00B total capacity over 0 modules (unprepared)\n\t\tNVMe: 97.66TB total capacity over 1 controller\n1.2.3.5:10001\n\tSummary:\n\t\tSCM: 0.00B total capacity over 0 modules (unprepared)\n\t\tNVMe: 97.66TB total capacity over 1 controller\n",
		},
		"nvme scan without health": {
			fmt.Sprint(MockScanResp(MockCtrlrs, nil, nil, MockServers, false)),
			"1.2.3.4:10000\n\tSCM Modules:\n\t\tnone\n\tNVMe controllers and namespaces:\n\t\tPCI Addr:0000:81:00.0 Serial:123ABC Model:ABC Fwrev:1.0.0 Socket:0\n\t\t\tNamespace: id:12345 capacity:97.66TB\n\tSummary:\n\t\tSCM: 0.00B total capacity over 0 modules (unprepared)\n\t\tNVMe: 97.66TB total capacity over 1 controller\n1.2.3.5:10001\n\tSCM Modules:\n\t\tnone\n\tNVMe controllers and namespaces:\n\t\tPCI Addr:0000:81:00.0 Serial:123ABC Model:ABC Fwrev:1.0.0 Socket:0\n\t\t\tNamespace: id:12345 capacity:97.66TB\n\tSummary:\n\t\tSCM: 0.00B total capacity over 0 modules (unprepared)\n\t\tNVMe: 97.66TB total capacity over 1 controller\n",
		},
		"nvme scan with health": {
			fmt.Sprint(MockScanResp(MockCtrlrs, nil, nil, MockServers, false).StringHealthStats()),
			"1.2.3.4:10000\n\tNVMe controllers and namespaces detail with health statistics:\n\t\tPCI Addr:0000:81:00.0 Serial:123ABC Model:ABC Fwrev:1.0.0 Socket:0\n\t\t\tNamespace: id:12345 capacity:97.66TB\n\t\tHealth Stats:\n\t\t\tTemperature:300K(27C)\n\t\t\tController Busy Time:0s\n\t\t\tPower Cycles:99\n\t\t\tPower On Duration:9999h0m0s\n\t\t\tUnsafe Shutdowns:1\n\t\t\tMedia Errors:0\n\t\t\tError Log Entries:0\n\t\t\tCritical Warnings:\n\t\t\t\tTemperature: OK\n\t\t\t\tAvailable Spare: OK\n\t\t\t\tDevice Reliability: OK\n\t\t\t\tRead Only: OK\n\t\t\t\tVolatile Memory Backup: OK\n1.2.3.5:10001\n\tNVMe controllers and namespaces detail with health statistics:\n\t\tPCI Addr:0000:81:00.0 Serial:123ABC Model:ABC Fwrev:1.0.0 Socket:0\n\t\t\tNamespace: id:12345 capacity:97.66TB\n\t\tHealth Stats:\n\t\t\tTemperature:300K(27C)\n\t\t\tController Busy Time:0s\n\t\t\tPower Cycles:99\n\t\t\tPower On Duration:9999h0m0s\n\t\t\tUnsafe Shutdowns:1\n\t\t\tMedia Errors:0\n\t\t\tError Log Entries:0\n\t\t\tCritical Warnings:\n\t\t\t\tTemperature: OK\n\t\t\t\tAvailable Spare: OK\n\t\t\t\tDevice Reliability: OK\n\t\t\t\tRead Only: OK\n\t\t\t\tVolatile Memory Backup: OK\n",
		},
		"scm scan summary with pmem namespaces": {
			fmt.Sprint(MockScanResp(nil, MockScmModules, MockScmNamespaces, MockServers, true)),
			"1.2.3.4:10000\n\tSummary:\n\t\tSCM: 2.90TB total capacity over 1 namespace\n\t\tNVMe: 0.00B total capacity over 0 controllers\n1.2.3.5:10001\n\tSummary:\n\t\tSCM: 2.90TB total capacity over 1 namespace\n\t\tNVMe: 0.00B total capacity over 0 controllers\n",
		},
		"scm scan with pmem namespaces": {
			fmt.Sprint(MockScanResp(nil, MockScmModules, MockScmNamespaces, MockServers, false)),
			"1.2.3.4:10000\n\tSCM Namespaces:\n\t\tDevice:pmem1 Socket:1 Capacity:2.90TB\n\tNVMe controllers and namespaces:\n\t\tnone\n\tSummary:\n\t\tSCM: 2.90TB total capacity over 1 namespace\n\t\tNVMe: 0.00B total capacity over 0 controllers\n1.2.3.5:10001\n\tSCM Namespaces:\n\t\tDevice:pmem1 Socket:1 Capacity:2.90TB\n\tNVMe controllers and namespaces:\n\t\tnone\n\tSummary:\n\t\tSCM: 2.90TB total capacity over 1 namespace\n\t\tNVMe: 0.00B total capacity over 0 controllers\n",
		},
		"scm scan summary without pmem namespaces": {
			fmt.Sprint(MockScanResp(nil, MockScmModules, nil, MockServers, true)),
			"1.2.3.4:10000\n\tSummary:\n\t\tSCM: 12.06KB total capacity over 1 module (unprepared)\n\t\tNVMe: 0.00B total capacity over 0 controllers\n1.2.3.5:10001\n\tSummary:\n\t\tSCM: 12.06KB total capacity over 1 module (unprepared)\n\t\tNVMe: 0.00B total capacity over 0 controllers\n",
		},
		"scm scan without pmem namespaces": {
			fmt.Sprint(MockScanResp(nil, MockScmModules, nil, MockServers, false)),
			"1.2.3.4:10000\n\tSCM Modules:\n\t\tPhysicalID:12345 Capacity:12.06KB Location:(socket:4 memctrlr:3 chan:1 pos:2)\n\tNVMe controllers and namespaces:\n\t\tnone\n\tSummary:\n\t\tSCM: 12.06KB total capacity over 1 module (unprepared)\n\t\tNVMe: 0.00B total capacity over 0 controllers\n1.2.3.5:10001\n\tSCM Modules:\n\t\tPhysicalID:12345 Capacity:12.06KB Location:(socket:4 memctrlr:3 chan:1 pos:2)\n\tNVMe controllers and namespaces:\n\t\tnone\n\tSummary:\n\t\tSCM: 12.06KB total capacity over 1 module (unprepared)\n\t\tNVMe: 0.00B total capacity over 0 controllers\n",
		},
		"scm mount scan": {
			NewClientScmMount(MockMounts, MockServers).String(),
			"1.2.3.4:10000:\n\tmntpoint:\"/mnt/daos\" \n\n1.2.3.5:10001:\n\tmntpoint:\"/mnt/daos\" \n\n",
		},
		"generic cmd results": {
			ResultMap{"1.2.3.4:10000": ClientResult{"1.2.3.4:10000", nil, MockErr}, "1.2.3.5:10001": ClientResult{"1.2.3.5:10001", nil, MockErr}}.String(),
			"1.2.3.4:10000:\n\terror: unknown failure\n1.2.3.5:10001:\n\terror: unknown failure\n",
		},
		"nvme operation results": {
			NewClientNvmeResults(
				[]*ctlpb.NvmeControllerResult{
					{
						Pciaddr: "0000:81:00.0",
						State: &ctlpb.ResponseState{
							Status: ctlpb.ResponseStatus_CTL_ERR_APP,
							Error:  "example application error",
						},
					},
				}, MockServers).String(),
			"1.2.3.4:10000:\n\tPCI Addr:0000:81:00.0 Status:CTL_ERR_APP Error:example application error\n\n1.2.3.5:10001:\n\tPCI Addr:0000:81:00.0 Status:CTL_ERR_APP Error:example application error\n\n",
		},
		"scm mountpoint operation results": {
			NewClientScmMountResults(
				[]*ctlpb.ScmMountResult{
					{
						Mntpoint: "/mnt/daos",
						State: &ctlpb.ResponseState{
							Status: ctlpb.ResponseStatus_CTL_ERR_APP,
							Error:  "example application error",
						},
					},
				}, MockServers).String(),
			"1.2.3.4:10000:\n\tMntpoint:/mnt/daos Status:CTL_ERR_APP Error:example application error\n\n1.2.3.5:10001:\n\tMntpoint:/mnt/daos Status:CTL_ERR_APP Error:example application error\n\n",
		},
	} {
		t.Run(name, func(t *testing.T) {
			if diff := cmp.Diff(tt.out, tt.m); diff != "" {
				t.Fatalf("unexpected output (-want, +got):\n%s\n", diff)
			}
		})
	}
}
