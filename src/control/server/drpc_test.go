//
// (C) Copyright 2019 Intel Corporation.
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

import (
	"net"
	"os"
	"path/filepath"
	"syscall"
	"testing"

	"github.com/pkg/errors"

	"github.com/daos-stack/daos/src/control/common"
	mgmtpb "github.com/daos-stack/daos/src/control/common/proto/mgmt"
	"github.com/daos-stack/daos/src/control/drpc"
)

func createTestSocket(t *testing.T, sockPath string) *net.UnixListener {
	addr := &net.UnixAddr{Name: sockPath, Net: "unixpacket"}
	sock, err := net.ListenUnix("unixpacket", addr)
	if err != nil {
		t.Fatalf("Couldn't set up test socket: %v", err)
	}

	err = os.Chmod(sockPath, 0777)
	if err != nil {
		cleanupTestSocket(sockPath, sock)
		t.Fatalf("Unable to set permissions on test socket: %v", err)
	}

	return sock
}

func cleanupTestSocket(path string, sock *net.UnixListener) {
	sock.Close()
	syscall.Unlink(path)
}

func TestCheckDrpcClientSocketPath_Empty(t *testing.T) {
	err := checkDrpcClientSocketPath("")

	if err == nil {
		t.Fatal("Expected an error, got nil")
	}
}

func TestCheckDrpcClientSocketPath_BadPath(t *testing.T) {
	err := checkDrpcClientSocketPath("/not/a/real/path")

	if err == nil {
		t.Fatal("Expected an error, got nil")
	}
}

func TestCheckDrpcClientSocketPath_DirNotSocket(t *testing.T) {
	tmpDir, tmpCleanup := common.CreateTestDir(t)
	defer tmpCleanup()

	path := filepath.Join(tmpDir, "drpc_test.sock")
	err := os.Mkdir(path, 0755)
	if err != nil {
		t.Fatalf("Failed to create directory: %v", err)
	}
	defer os.Remove(path)

	err = checkDrpcClientSocketPath(path)

	if err == nil {
		t.Fatal("Expected an error, got nil")
	}
}

func TestCheckDrpcClientSocketPath_FileNotSocket(t *testing.T) {
	tmpDir, tmpCleanup := common.CreateTestDir(t)
	defer tmpCleanup()

	path := filepath.Join(tmpDir, "drpc_test.sock")
	f, err := os.Create(path)
	if err != nil {
		t.Fatalf("Failed to create temp file: %v", err)
	}
	f.Close()
	defer os.Remove(path)

	err = checkDrpcClientSocketPath(path)

	if err == nil {
		t.Fatal("Expected an error, got nil")
	}
}

func TestCheckDrpcClientSocketPath_Success(t *testing.T) {
	tmpDir, tmpCleanup := common.CreateTestDir(t)
	defer tmpCleanup()

	path := filepath.Join(tmpDir, "drpc_test.sock")
	sock := createTestSocket(t, path)
	defer cleanupTestSocket(path, sock)

	err := checkDrpcClientSocketPath(path)

	if err != nil {
		t.Fatalf("Expected no error, got error: %v", err)
	}
}

func TestGetDrpcServerSocketPath_EmptyString(t *testing.T) {
	expectedPath := "daos_server.sock"

	path := getDrpcServerSocketPath("")

	if path != expectedPath {
		t.Errorf("Expected %q, got %q", expectedPath, path)
	}
}

func TestGetDrpcServerSocketPath(t *testing.T) {
	dirPath := "/some/server/dir"
	expectedPath := filepath.Join(dirPath, "daos_server.sock")

	path := getDrpcServerSocketPath(dirPath)

	if path != expectedPath {
		t.Errorf("Expected %q, got %q", expectedPath, path)
	}
}

func TestDrpcCleanup_BadSocketDir(t *testing.T) {
	badDir := "/some/fake/path"

	err := drpcCleanup(badDir)

	if err == nil {
		t.Fatal("Expected error, got nil")
	}
}

func TestDrpcCleanup_EmptyDir(t *testing.T) {
	tmpDir, tmpCleanup := common.CreateTestDir(t)
	defer tmpCleanup()

	err := drpcCleanup(tmpDir)

	if err != nil {
		t.Fatalf("Expected no error, got: %v", err)
	}
}

func expectDoesNotExist(t *testing.T, path string) {
	if _, err := os.Stat(path); err == nil || !os.IsNotExist(err) {
		t.Errorf("expected %q to no longer exist, but got error: %v",
			path, err)
	}
}

func TestDrpcCleanup_Single(t *testing.T) {
	tmpDir, tmpCleanup := common.CreateTestDir(t)
	defer tmpCleanup()

	for _, sockName := range []string{
		"daos_server.sock",
		"daos_io_server.sock",
		"daos_io_server0.sock",
		"daos_io_server_2345.sock",
	} {
		sockPath := filepath.Join(tmpDir, sockName)
		sock := createTestSocket(t, sockPath)
		defer cleanupTestSocket(sockPath, sock)

		err := drpcCleanup(tmpDir)

		if err != nil {
			t.Fatalf("%q: Expected no error, got: %v", sockPath, err)
		}

		expectDoesNotExist(t, sockPath)
	}
}

func TestDrpcCleanup_DoesNotDeleteNonDaosSocketFiles(t *testing.T) {
	tmpDir, tmpCleanup := common.CreateTestDir(t)
	defer tmpCleanup()

	for _, sockName := range []string{
		"someone_else.sock",
		"12345.sock",
		"myfile",
		"daos_server",
		"daos_io_server",
	} {
		sockPath := filepath.Join(tmpDir, sockName)
		sock := createTestSocket(t, sockPath)
		defer cleanupTestSocket(sockPath, sock)

		err := drpcCleanup(tmpDir)

		if err != nil {
			t.Fatalf("%q: Expected no error, got: %v", sockPath, err)
		}

		if _, err := os.Stat(sockPath); err != nil {
			t.Errorf("expected %q to exist, but got error: %v",
				sockPath, err)
		}
	}
}

func TestDrpcCleanup_Multiple(t *testing.T) {
	tmpDir, tmpCleanup := common.CreateTestDir(t)
	defer tmpCleanup()

	sockNames := []string{
		"daos_server.sock",
		"daos_io_server.sock",
		"daos_io_server12.sock",
		"daos_io_serverF.sock",
		"daos_io_server_5678.sock",
		"daos_io_server_256.sock",
		"daos_io_server_abc.sock",
	}

	var sockPaths []string

	for _, sockName := range sockNames {
		path := filepath.Join(tmpDir, sockName)
		sockPaths = append(sockPaths, path)

		sock := createTestSocket(t, path)
		defer cleanupTestSocket(path, sock)
	}

	err := drpcCleanup(tmpDir)

	if err != nil {
		t.Fatalf("Expected no error, got: %v", err)
	}

	for _, path := range sockPaths {
		expectDoesNotExist(t, path)
	}
}

func TestDrpc_Errors(t *testing.T) {
	for name, tc := range map[string]struct {
		notReady     bool
		connectError error
		sendError    error
		resp         *drpc.Response
		expErr       error
	}{
		"connect fails": {
			connectError: errors.New("connect"),
			expErr:       errors.New("connect"),
		},
		"send msg fails": {
			sendError: errors.New("send"),
			expErr:    errors.New("send"),
		},
		"nil resp": {
			expErr: errors.New("no response"),
		},
		"failed status": {
			resp: &drpc.Response{
				Status: drpc.Status_FAILURE,
			},
			expErr: errors.New("status: FAILURE"),
		},
		"success": {
			resp: &drpc.Response{},
		},
	} {
		t.Run(name, func(t *testing.T) {
			cfg := &mockDrpcClientConfig{
				SendMsgError:    tc.sendError,
				SendMsgResponse: tc.resp,
				ConnectError:    tc.connectError,
			}
			mc := newMockDrpcClient(cfg)

			_, err := makeDrpcCall(mc, drpc.ModuleMgmt, drpc.MethodPoolCreate, &mgmtpb.PoolCreateReq{})
			common.CmpErr(t, tc.expErr, err)
		})
	}
}
