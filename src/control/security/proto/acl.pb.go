// Code generated by protoc-gen-go. DO NOT EDIT.
// source: acl.proto

package proto

import proto "github.com/golang/protobuf/proto"
import fmt "fmt"
import math "math"

import (
	context "golang.org/x/net/context"
	grpc "google.golang.org/grpc"
)

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.ProtoPackageIsVersion2 // please upgrade the proto package

type AclRequestStatus int32

const (
	AclRequestStatus_SUCCESS               AclRequestStatus = 0
	AclRequestStatus_ERR_UNKNOWN           AclRequestStatus = -1
	AclRequestStatus_ERR_PERM_DENIED       AclRequestStatus = -2
	AclRequestStatus_ERR_INVALID_PERMS     AclRequestStatus = -3
	AclRequestStatus_ERR_INVALID_PRINCIPAL AclRequestStatus = -4
	AclRequestStatus_ERR_INVALID_UUID      AclRequestStatus = -5
)

var AclRequestStatus_name = map[int32]string{
	0:  "SUCCESS",
	-1: "ERR_UNKNOWN",
	-2: "ERR_PERM_DENIED",
	-3: "ERR_INVALID_PERMS",
	-4: "ERR_INVALID_PRINCIPAL",
	-5: "ERR_INVALID_UUID",
}
var AclRequestStatus_value = map[string]int32{
	"SUCCESS":               0,
	"ERR_UNKNOWN":           -1,
	"ERR_PERM_DENIED":       -2,
	"ERR_INVALID_PERMS":     -3,
	"ERR_INVALID_PRINCIPAL": -4,
	"ERR_INVALID_UUID":      -5,
}

func (x AclRequestStatus) String() string {
	return proto.EnumName(AclRequestStatus_name, int32(x))
}
func (AclRequestStatus) EnumDescriptor() ([]byte, []int) {
	return fileDescriptor_acl_08c19ec25d13204d, []int{0}
}

// Bits representing access permissions
type AclPermissions int32

const (
	AclPermissions_NO_ACCESS AclPermissions = 0
	AclPermissions_READ      AclPermissions = 1
	AclPermissions_WRITE     AclPermissions = 2
)

var AclPermissions_name = map[int32]string{
	0: "NO_ACCESS",
	1: "READ",
	2: "WRITE",
}
var AclPermissions_value = map[string]int32{
	"NO_ACCESS": 0,
	"READ":      1,
	"WRITE":     2,
}

func (x AclPermissions) String() string {
	return proto.EnumName(AclPermissions_name, int32(x))
}
func (AclPermissions) EnumDescriptor() ([]byte, []int) {
	return fileDescriptor_acl_08c19ec25d13204d, []int{1}
}

// A given user/group may have multiple different types of entries
type AclEntryType int32

const (
	AclEntryType_ALLOW AclEntryType = 0
	AclEntryType_AUDIT AclEntryType = 1
	AclEntryType_ALARM AclEntryType = 2
)

var AclEntryType_name = map[int32]string{
	0: "ALLOW",
	1: "AUDIT",
	2: "ALARM",
}
var AclEntryType_value = map[string]int32{
	"ALLOW": 0,
	"AUDIT": 1,
	"ALARM": 2,
}

func (x AclEntryType) String() string {
	return proto.EnumName(AclEntryType_name, int32(x))
}
func (AclEntryType) EnumDescriptor() ([]byte, []int) {
	return fileDescriptor_acl_08c19ec25d13204d, []int{2}
}

// Bits representing flags on a given ACL entry
type AclFlags int32

const (
	AclFlags_NO_FLAGS       AclFlags = 0
	AclFlags_GROUP          AclFlags = 1
	AclFlags_ACCESS_SUCCESS AclFlags = 2
	AclFlags_ACCESS_FAILURE AclFlags = 4
	AclFlags_POOL_INHERIT   AclFlags = 8
)

var AclFlags_name = map[int32]string{
	0: "NO_FLAGS",
	1: "GROUP",
	2: "ACCESS_SUCCESS",
	4: "ACCESS_FAILURE",
	8: "POOL_INHERIT",
}
var AclFlags_value = map[string]int32{
	"NO_FLAGS":       0,
	"GROUP":          1,
	"ACCESS_SUCCESS": 2,
	"ACCESS_FAILURE": 4,
	"POOL_INHERIT":   8,
}

func (x AclFlags) String() string {
	return proto.EnumName(AclFlags_name, int32(x))
}
func (AclFlags) EnumDescriptor() ([]byte, []int) {
	return fileDescriptor_acl_08c19ec25d13204d, []int{3}
}

type AclResponse struct {
	Status               AclRequestStatus     `protobuf:"varint,1,opt,name=status,proto3,enum=proto.AclRequestStatus" json:"status,omitempty"`
	Permissions          *AclEntryPermissions `protobuf:"bytes,2,opt,name=permissions,proto3" json:"permissions,omitempty"`
	XXX_NoUnkeyedLiteral struct{}             `json:"-"`
	XXX_unrecognized     []byte               `json:"-"`
	XXX_sizecache        int32                `json:"-"`
}

func (m *AclResponse) Reset()         { *m = AclResponse{} }
func (m *AclResponse) String() string { return proto.CompactTextString(m) }
func (*AclResponse) ProtoMessage()    {}
func (*AclResponse) Descriptor() ([]byte, []int) {
	return fileDescriptor_acl_08c19ec25d13204d, []int{0}
}
func (m *AclResponse) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_AclResponse.Unmarshal(m, b)
}
func (m *AclResponse) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_AclResponse.Marshal(b, m, deterministic)
}
func (dst *AclResponse) XXX_Merge(src proto.Message) {
	xxx_messageInfo_AclResponse.Merge(dst, src)
}
func (m *AclResponse) XXX_Size() int {
	return xxx_messageInfo_AclResponse.Size(m)
}
func (m *AclResponse) XXX_DiscardUnknown() {
	xxx_messageInfo_AclResponse.DiscardUnknown(m)
}

var xxx_messageInfo_AclResponse proto.InternalMessageInfo

func (m *AclResponse) GetStatus() AclRequestStatus {
	if m != nil {
		return m.Status
	}
	return AclRequestStatus_SUCCESS
}

func (m *AclResponse) GetPermissions() *AclEntryPermissions {
	if m != nil {
		return m.Permissions
	}
	return nil
}

// Identifier for a specific Access Control Entry
type AclEntry struct {
	Type                 AclEntryType `protobuf:"varint,1,opt,name=type,proto3,enum=proto.AclEntryType" json:"type,omitempty"`
	Flags                uint32       `protobuf:"varint,2,opt,name=flags,proto3" json:"flags,omitempty"`
	Entity               string       `protobuf:"bytes,3,opt,name=entity,proto3" json:"entity,omitempty"`
	Identity             string       `protobuf:"bytes,4,opt,name=identity,proto3" json:"identity,omitempty"`
	XXX_NoUnkeyedLiteral struct{}     `json:"-"`
	XXX_unrecognized     []byte       `json:"-"`
	XXX_sizecache        int32        `json:"-"`
}

func (m *AclEntry) Reset()         { *m = AclEntry{} }
func (m *AclEntry) String() string { return proto.CompactTextString(m) }
func (*AclEntry) ProtoMessage()    {}
func (*AclEntry) Descriptor() ([]byte, []int) {
	return fileDescriptor_acl_08c19ec25d13204d, []int{1}
}
func (m *AclEntry) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_AclEntry.Unmarshal(m, b)
}
func (m *AclEntry) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_AclEntry.Marshal(b, m, deterministic)
}
func (dst *AclEntry) XXX_Merge(src proto.Message) {
	xxx_messageInfo_AclEntry.Merge(dst, src)
}
func (m *AclEntry) XXX_Size() int {
	return xxx_messageInfo_AclEntry.Size(m)
}
func (m *AclEntry) XXX_DiscardUnknown() {
	xxx_messageInfo_AclEntry.DiscardUnknown(m)
}

var xxx_messageInfo_AclEntry proto.InternalMessageInfo

func (m *AclEntry) GetType() AclEntryType {
	if m != nil {
		return m.Type
	}
	return AclEntryType_ALLOW
}

func (m *AclEntry) GetFlags() uint32 {
	if m != nil {
		return m.Flags
	}
	return 0
}

func (m *AclEntry) GetEntity() string {
	if m != nil {
		return m.Entity
	}
	return ""
}

func (m *AclEntry) GetIdentity() string {
	if m != nil {
		return m.Identity
	}
	return ""
}

// Permissions for the given entry
type AclEntryPermissions struct {
	Entry                *AclEntry `protobuf:"bytes,1,opt,name=entry,proto3" json:"entry,omitempty"`
	PermissionBits       uint64    `protobuf:"varint,2,opt,name=permission_bits,json=permissionBits,proto3" json:"permission_bits,omitempty"`
	XXX_NoUnkeyedLiteral struct{}  `json:"-"`
	XXX_unrecognized     []byte    `json:"-"`
	XXX_sizecache        int32     `json:"-"`
}

func (m *AclEntryPermissions) Reset()         { *m = AclEntryPermissions{} }
func (m *AclEntryPermissions) String() string { return proto.CompactTextString(m) }
func (*AclEntryPermissions) ProtoMessage()    {}
func (*AclEntryPermissions) Descriptor() ([]byte, []int) {
	return fileDescriptor_acl_08c19ec25d13204d, []int{2}
}
func (m *AclEntryPermissions) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_AclEntryPermissions.Unmarshal(m, b)
}
func (m *AclEntryPermissions) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_AclEntryPermissions.Marshal(b, m, deterministic)
}
func (dst *AclEntryPermissions) XXX_Merge(src proto.Message) {
	xxx_messageInfo_AclEntryPermissions.Merge(dst, src)
}
func (m *AclEntryPermissions) XXX_Size() int {
	return xxx_messageInfo_AclEntryPermissions.Size(m)
}
func (m *AclEntryPermissions) XXX_DiscardUnknown() {
	xxx_messageInfo_AclEntryPermissions.DiscardUnknown(m)
}

var xxx_messageInfo_AclEntryPermissions proto.InternalMessageInfo

func (m *AclEntryPermissions) GetEntry() *AclEntry {
	if m != nil {
		return m.Entry
	}
	return nil
}

func (m *AclEntryPermissions) GetPermissionBits() uint64 {
	if m != nil {
		return m.PermissionBits
	}
	return 0
}

func init() {
	proto.RegisterType((*AclResponse)(nil), "proto.AclResponse")
	proto.RegisterType((*AclEntry)(nil), "proto.AclEntry")
	proto.RegisterType((*AclEntryPermissions)(nil), "proto.AclEntryPermissions")
	proto.RegisterEnum("proto.AclRequestStatus", AclRequestStatus_name, AclRequestStatus_value)
	proto.RegisterEnum("proto.AclPermissions", AclPermissions_name, AclPermissions_value)
	proto.RegisterEnum("proto.AclEntryType", AclEntryType_name, AclEntryType_value)
	proto.RegisterEnum("proto.AclFlags", AclFlags_name, AclFlags_value)
}

// Reference imports to suppress errors if they are not otherwise used.
var _ context.Context
var _ grpc.ClientConn

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
const _ = grpc.SupportPackageIsVersion4

// AccessControlClient is the client API for AccessControl service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://godoc.org/google.golang.org/grpc#ClientConn.NewStream.
type AccessControlClient interface {
	// Set the permissions on a given ACE or create it if it doesn't exist
	SetPermissions(ctx context.Context, in *AclEntryPermissions, opts ...grpc.CallOption) (*AclResponse, error)
	// Fetch the permissions on a given ACE
	GetPermissions(ctx context.Context, in *AclEntry, opts ...grpc.CallOption) (*AclResponse, error)
	// Remove the given ACE completely from the ACL
	DestroyAclEntry(ctx context.Context, in *AclEntry, opts ...grpc.CallOption) (*AclResponse, error)
}

type accessControlClient struct {
	cc *grpc.ClientConn
}

func NewAccessControlClient(cc *grpc.ClientConn) AccessControlClient {
	return &accessControlClient{cc}
}

func (c *accessControlClient) SetPermissions(ctx context.Context, in *AclEntryPermissions, opts ...grpc.CallOption) (*AclResponse, error) {
	out := new(AclResponse)
	err := c.cc.Invoke(ctx, "/proto.AccessControl/SetPermissions", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *accessControlClient) GetPermissions(ctx context.Context, in *AclEntry, opts ...grpc.CallOption) (*AclResponse, error) {
	out := new(AclResponse)
	err := c.cc.Invoke(ctx, "/proto.AccessControl/GetPermissions", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *accessControlClient) DestroyAclEntry(ctx context.Context, in *AclEntry, opts ...grpc.CallOption) (*AclResponse, error) {
	out := new(AclResponse)
	err := c.cc.Invoke(ctx, "/proto.AccessControl/DestroyAclEntry", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

// AccessControlServer is the server API for AccessControl service.
type AccessControlServer interface {
	// Set the permissions on a given ACE or create it if it doesn't exist
	SetPermissions(context.Context, *AclEntryPermissions) (*AclResponse, error)
	// Fetch the permissions on a given ACE
	GetPermissions(context.Context, *AclEntry) (*AclResponse, error)
	// Remove the given ACE completely from the ACL
	DestroyAclEntry(context.Context, *AclEntry) (*AclResponse, error)
}

func RegisterAccessControlServer(s *grpc.Server, srv AccessControlServer) {
	s.RegisterService(&_AccessControl_serviceDesc, srv)
}

func _AccessControl_SetPermissions_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(AclEntryPermissions)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(AccessControlServer).SetPermissions(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/proto.AccessControl/SetPermissions",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(AccessControlServer).SetPermissions(ctx, req.(*AclEntryPermissions))
	}
	return interceptor(ctx, in, info, handler)
}

func _AccessControl_GetPermissions_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(AclEntry)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(AccessControlServer).GetPermissions(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/proto.AccessControl/GetPermissions",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(AccessControlServer).GetPermissions(ctx, req.(*AclEntry))
	}
	return interceptor(ctx, in, info, handler)
}

func _AccessControl_DestroyAclEntry_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(AclEntry)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(AccessControlServer).DestroyAclEntry(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/proto.AccessControl/DestroyAclEntry",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(AccessControlServer).DestroyAclEntry(ctx, req.(*AclEntry))
	}
	return interceptor(ctx, in, info, handler)
}

var _AccessControl_serviceDesc = grpc.ServiceDesc{
	ServiceName: "proto.AccessControl",
	HandlerType: (*AccessControlServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "SetPermissions",
			Handler:    _AccessControl_SetPermissions_Handler,
		},
		{
			MethodName: "GetPermissions",
			Handler:    _AccessControl_GetPermissions_Handler,
		},
		{
			MethodName: "DestroyAclEntry",
			Handler:    _AccessControl_DestroyAclEntry_Handler,
		},
	},
	Streams:  []grpc.StreamDesc{},
	Metadata: "acl.proto",
}

func init() { proto.RegisterFile("acl.proto", fileDescriptor_acl_08c19ec25d13204d) }

var fileDescriptor_acl_08c19ec25d13204d = []byte{
	// 522 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0x8c, 0x93, 0x5d, 0x8f, 0xd2, 0x4c,
	0x14, 0xc7, 0x29, 0x0f, 0xf0, 0xc0, 0x29, 0x94, 0x71, 0xd6, 0x17, 0x42, 0xd4, 0x10, 0x12, 0xb3,
	0x84, 0x8b, 0xdd, 0x04, 0x4d, 0xf4, 0xc2, 0x9b, 0x59, 0x3a, 0xe0, 0xc4, 0x6e, 0x4b, 0xa6, 0x54,
	0xae, 0x4c, 0xc3, 0xe2, 0x68, 0x9a, 0x20, 0x45, 0x66, 0xf6, 0xa2, 0x89, 0x7e, 0x31, 0x3f, 0x80,
	0x5f, 0xca, 0xd7, 0x30, 0x05, 0xda, 0x25, 0x6a, 0xec, 0x4d, 0xe7, 0x9c, 0xff, 0xef, 0xcc, 0x39,
	0x73, 0xe6, 0x0c, 0xd4, 0xe6, 0x8b, 0xe5, 0xd9, 0x7a, 0x13, 0xab, 0x18, 0x97, 0xf5, 0xaf, 0xfb,
	0x11, 0x4c, 0xb2, 0x58, 0x72, 0x21, 0xd7, 0xf1, 0x4a, 0x0a, 0x7c, 0x0e, 0x15, 0xa9, 0xe6, 0xea,
	0x5a, 0xb6, 0x8c, 0x8e, 0xd1, 0xb3, 0x06, 0xf7, 0x52, 0xfa, 0x4c, 0x33, 0x1f, 0xae, 0x85, 0x54,
	0xbe, 0x96, 0xf9, 0x0e, 0xc3, 0xcf, 0xc1, 0x5c, 0x8b, 0xcd, 0xfb, 0x48, 0xca, 0x28, 0x5e, 0xc9,
	0x56, 0xb1, 0x63, 0xf4, 0xcc, 0x41, 0x3b, 0x8b, 0xa2, 0x2b, 0xb5, 0x49, 0x26, 0x19, 0xc1, 0xf3,
	0x78, 0xf7, 0x13, 0x54, 0xf7, 0x0c, 0x3e, 0x85, 0x92, 0x4a, 0xd6, 0x62, 0x97, 0xf8, 0xe4, 0x68,
	0x8b, 0x69, 0xb2, 0x16, 0x5c, 0x03, 0xf8, 0x36, 0x94, 0xdf, 0x2e, 0xe7, 0xef, 0xd2, 0x64, 0x0d,
	0x9e, 0x1a, 0xf8, 0x2e, 0x54, 0xc4, 0x4a, 0x45, 0x2a, 0x69, 0xfd, 0xd7, 0x31, 0x7a, 0x35, 0xbe,
	0xb3, 0x70, 0x1b, 0xaa, 0xd1, 0x9b, 0x9d, 0x52, 0xd2, 0xca, 0xc1, 0xee, 0x0a, 0x38, 0xf9, 0x4d,
	0x89, 0xf8, 0x11, 0x94, 0xc5, 0xd6, 0xa7, 0x4b, 0x31, 0x07, 0xcd, 0xa3, 0x52, 0x78, 0xaa, 0xe2,
	0x53, 0x68, 0x66, 0x67, 0x09, 0xaf, 0x22, 0x95, 0x56, 0x54, 0xe2, 0x56, 0xe6, 0xbe, 0x88, 0x94,
	0xec, 0x7f, 0x36, 0x00, 0x1d, 0x37, 0x10, 0x9b, 0xf0, 0xbf, 0x1f, 0x0c, 0x87, 0xd4, 0xf7, 0x51,
	0x01, 0xb7, 0xc0, 0xa4, 0x9c, 0x87, 0x81, 0xfb, 0xd2, 0xf5, 0x66, 0x2e, 0xfa, 0xb9, 0xff, 0x0c,
	0x7c, 0x1f, 0x9a, 0x5b, 0x65, 0x42, 0xf9, 0x65, 0x68, 0x53, 0x97, 0x51, 0x1b, 0xfd, 0xc8, 0xd4,
	0x87, 0x70, 0x6b, 0xab, 0x32, 0xf7, 0x15, 0x71, 0x98, 0xad, 0x29, 0x1f, 0x7d, 0xcf, 0xf4, 0x2e,
	0xdc, 0xb9, 0xa1, 0x73, 0xe6, 0x0e, 0xd9, 0x84, 0x38, 0xe8, 0x5b, 0xc6, 0x3c, 0x00, 0x94, 0x67,
	0x82, 0x80, 0xd9, 0xe8, 0xeb, 0x41, 0xee, 0x3f, 0x01, 0x8b, 0x2c, 0x96, 0xf9, 0xf6, 0x34, 0xa0,
	0xe6, 0x7a, 0x21, 0xd9, 0xd7, 0x5e, 0x85, 0x12, 0xa7, 0xc4, 0x46, 0x06, 0xae, 0x41, 0x79, 0xc6,
	0xd9, 0x94, 0xa2, 0x62, 0xff, 0x1c, 0xea, 0xf9, 0x9b, 0xdb, 0x4a, 0xc4, 0x71, 0xbc, 0x19, 0x2a,
	0xe8, 0x65, 0x60, 0xb3, 0x69, 0x1a, 0x40, 0x1c, 0xc2, 0x2f, 0x51, 0xb1, 0xff, 0x5a, 0x4f, 0xc2,
	0x48, 0x5f, 0x65, 0x1d, 0xaa, 0xae, 0x17, 0x8e, 0x1c, 0x32, 0xf6, 0x53, 0x7e, 0xcc, 0xbd, 0x60,
	0x82, 0x0c, 0x8c, 0xc1, 0x4a, 0xd3, 0x86, 0xfb, 0xd6, 0x15, 0x73, 0xbe, 0x11, 0x61, 0x4e, 0xc0,
	0x29, 0x2a, 0x61, 0x04, 0xf5, 0x89, 0xe7, 0x39, 0x21, 0x73, 0x5f, 0x50, 0xce, 0xa6, 0xa8, 0x3a,
	0xf8, 0x62, 0x40, 0x83, 0x2c, 0x16, 0x42, 0xca, 0x61, 0xbc, 0x52, 0x9b, 0x78, 0x89, 0x2f, 0xc0,
	0xf2, 0x85, 0xca, 0x9f, 0xeb, 0x2f, 0x53, 0xdb, 0xc6, 0xf9, 0x77, 0x90, 0xbe, 0x95, 0x6e, 0x01,
	0x3f, 0x05, 0x6b, 0x7c, 0x73, 0x8f, 0xe3, 0x59, 0xf9, 0x43, 0xe0, 0x33, 0x68, 0xda, 0x42, 0xaa,
	0x4d, 0x9c, 0x1c, 0xc6, 0xff, 0xdf, 0x22, 0xaf, 0x2a, 0xda, 0xf9, 0xf8, 0x57, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xf5, 0x62, 0xab, 0xeb, 0xca, 0x03, 0x00, 0x00,
}