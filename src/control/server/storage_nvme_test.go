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

package server

/*
func TestFormatNvme(t *testing.T) {
	pciAddr := "0000:81:00.0"
	model := "ABC"
	serial := "123ABC"
	fwRev := "1.0.0"
	newDefaultCtrlrs := func() NvmeControllers {
		return NvmeControllers{
			common.NewMockControllerPB(pciAddr, fwRev, model, serial,
				NvmeNamespaces(nil), nil),
		}
	}

	tests := []struct {
		formatted    bool
		devFormatRet error
		pciAddrs     []string
		expResults   NvmeControllerResults
		expCtrlrs    NvmeControllers
		desc         string
	}{
		{
			formatted:    false,
			devFormatRet: nil,
			pciAddrs:     []string{},
			expResults: NvmeControllerResults{
				{
					Pciaddr: "",
					State: &ResponseState{
						Status: ResponseStatus_CTL_SUCCESS,
						Info:   msgBdevNoDevs,
					},
				},
			},
			expCtrlrs: newDefaultCtrlrs(),
			desc:      "no devices",
		},
		{
			formatted:    true,
			devFormatRet: nil,
			pciAddrs:     []string{},
			expResults: NvmeControllerResults{
				{
					Pciaddr: "",
					State: &ResponseState{
						Status: ResponseStatus_CTL_ERR_APP,
						Error:  msgBdevAlreadyFormatted,
					},
				},
			},
			expCtrlrs: newDefaultCtrlrs(),
			desc:      "already formatted",
		},
		{
			formatted:    false,
			devFormatRet: nil,
			pciAddrs:     []string{""},
			expResults: NvmeControllerResults{
				{
					Pciaddr: "",
					State: &ResponseState{
						Status: ResponseStatus_CTL_ERR_CONF,
						Error:  msgBdevEmpty,
					},
				},
			},
			expCtrlrs: newDefaultCtrlrs(),
			desc:      "empty device string",
		},
		{
			formatted:    false,
			devFormatRet: nil,
			pciAddrs:     []string{"0000:81:00.0"},
			expResults: NvmeControllerResults{
				{
					Pciaddr: "0000:81:00.0",
					State:   new(ResponseState),
				},
			},
			expCtrlrs: newDefaultCtrlrs(),
			desc:      "single device",
		},
		{
			formatted:    false,
			devFormatRet: nil,
			pciAddrs:     []string{"0000:83:00.0"},
			expResults: NvmeControllerResults{
				{
					Pciaddr: "0000:83:00.0",
					State: &ResponseState{
						Status: ResponseStatus_CTL_ERR_NVME,
						Error:  "0000:83:00.0: " + msgBdevNotFound,
					},
				},
			},
			expCtrlrs: newDefaultCtrlrs(),
			desc:      "single device not discovered",
		},
		{
			formatted:    false,
			devFormatRet: nil,
			pciAddrs:     []string{"0000:81:00.0", "0000:83:00.0"},
			expResults: NvmeControllerResults{
				{
					Pciaddr: "0000:81:00.0",
					State:   new(ResponseState),
				},
				{
					Pciaddr: "0000:83:00.0",
					State: &ResponseState{
						Status: ResponseStatus_CTL_ERR_NVME,
						Error:  "0000:83:00.0: " + msgBdevNotFound,
					},
				},
			},
			expCtrlrs: newDefaultCtrlrs(),
			desc:      "first device found, second not discovered",
		},
		{
			formatted:    false,
			devFormatRet: nil,
			pciAddrs:     []string{"0000:83:00.0", "0000:81:00.0"},
			expResults: NvmeControllerResults{
				{
					Pciaddr: "0000:83:00.0",
					State: &ResponseState{
						Status: ResponseStatus_CTL_ERR_NVME,
						Error:  "0000:83:00.0: " + msgBdevNotFound,
					},
				},
				{
					Pciaddr: "0000:81:00.0",
					State:   new(ResponseState),
				},
			},
			expCtrlrs: newDefaultCtrlrs(),
			desc:      "first not discovered, second found",
		},
		{
			formatted:    false,
			devFormatRet: errors.New("example format failure"),
			pciAddrs:     []string{"0000:83:00.0", "0000:81:00.0"},
			expResults: NvmeControllerResults{
				{
					Pciaddr: "0000:83:00.0",
					State: &ResponseState{
						Status: ResponseStatus_CTL_ERR_NVME,
						Error:  "0000:83:00.0: " + msgBdevNotFound,
					},
				},
				{
					Pciaddr: "0000:81:00.0",
					State: &ResponseState{
						Status: ResponseStatus_CTL_ERR_NVME,
						Error: "0000:81:00.0: " +
							"example format failure",
					},
				},
			},
			expCtrlrs: newDefaultCtrlrs(),
			desc:      "first not discovered, second failed to format",
		},
	}

	srvIdx := 0 // assume just a single io_server (index 0)

	for _, tt := range tests {
		t.Run(tt.desc, func(t *testing.T) {
			log, buf := logging.NewTestLogger(t.Name())
			defer common.ShowBufferOnFailure(t, buf)

			nvmeFormatCalls = []string{}

			config := defaultMockConfig(t)
			bdCfg := config.Servers[srvIdx].Storage.Bdev
			bdCfg.DeviceList = tt.pciAddrs

			c := MockController()
			// create nvmeStorage struct with customised test behaviour
			sn := newMockNvmeStorage(log, &mockExt{}, defaultMockSpdkEnv(),
				newMockSpdkNvme(log, []Controller{c}, []Namespace{},
					[]DeviceHealth{}, nil, tt.devFormatRet),
				false)
			sn.formatted = tt.formatted

			results := NvmeControllerResults{}

			if err := sn.Discover(); err != nil {
				t.Fatal(err)
			}

			sn.Format(bdCfg, &results)

			common.AssertEqual(
				t, len(results), len(tt.expResults),
				"unexpected number of response results, "+tt.desc)

			successPciaddrs := []string{}
			for i, result := range results {
				common.AssertEqual(
					t, result.State.Status, tt.expResults[i].State.Status,
					"unexpected response status, "+tt.desc)
				common.AssertEqual(
					t, result.State.Error, tt.expResults[i].State.Error,
					"unexpected result error message, "+tt.desc)
				common.AssertEqual(
					t, result.Pciaddr, tt.expResults[i].Pciaddr,
					"unexpected pciaddr, "+tt.desc)

				if result.State.Status == ResponseStatus_CTL_SUCCESS {
					if result.State.Info != msgBdevNoDevs {
						successPciaddrs = append(successPciaddrs, result.Pciaddr)
					}
				}
			}

			common.AssertEqual(
				t, nvmeFormatCalls, successPciaddrs,
				"unexpected list of pci addresses in format calls, "+tt.desc)
			common.AssertEqual(t, sn.formatted, true, "expect formatted state, "+tt.desc)

			common.AssertEqual(
				t, sn.controllers[0], tt.expCtrlrs[0],
				"unexpected list of discovered controllers, "+tt.desc)
		})
	}
}
*/
