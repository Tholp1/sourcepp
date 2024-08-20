using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace vpkpp.Format
{
    using EntryCallback = Action<string, Entry>;

    internal static unsafe partial class Extern
    {
        [DllImport("vpkppc")]
        public static extern void* vpkpp_vpk_create_empty([MarshalAs(UnmanagedType.LPStr)] string path);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_vpk_create_empty_with_options([MarshalAs(UnmanagedType.LPStr)] string path, PackFileOptions options);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_vpk_create_from_directory([MarshalAs(UnmanagedType.LPStr)] string vpkPath, [MarshalAs(UnmanagedType.LPStr)] string contentPath, byte saveToDir);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_vpk_create_from_directory_with_options([MarshalAs(UnmanagedType.LPStr)] string vpkPath, [MarshalAs(UnmanagedType.LPStr)] string contentPath, byte saveToDir, PackFileOptions options);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_vpk_open([MarshalAs(UnmanagedType.LPStr)] string path);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_vpk_open_with_callback([MarshalAs(UnmanagedType.LPStr)] string path, IntPtr callback);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_vpk_open_with_options([MarshalAs(UnmanagedType.LPStr)] string path, PackFileOptions options);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_vpk_open_with_options_and_callback([MarshalAs(UnmanagedType.LPStr)] string path, PackFileOptions options, IntPtr callback);

        [DllImport("vpkppc")]
        public static extern byte vpkpp_vpk_generate_keypair_files([MarshalAs(UnmanagedType.LPStr)] string path);

        [DllImport("vpkppc")]
        public static extern byte vpkpp_vpk_sign_from_file(void* handle, [MarshalAs(UnmanagedType.LPStr)] string filepath);

        [DllImport("vpkppc")]
        public static extern byte vpkpp_vpk_sign_from_mem(void* handle, byte* privateKeyBuffer, ulong privateKeyLen, byte* publicKeyBuffer, ulong publicKeyLen);

        [DllImport("vpkppc")]
        public static extern uint vpkpp_vpk_get_version(void* handle);

        [DllImport("vpkppc")]
        public static extern void vpkpp_vpk_set_version(void* handle, uint version);
    }

    public class VPK : PackFile
    {
        private unsafe VPK(void* handle) : base(handle) {}

        public static VPK? CreateEmpty(string path)
        {
            unsafe
            {
                var handle = Extern.vpkpp_vpk_create_empty(path);
                return handle == null ? null : new VPK(handle);
            }
        }

        public static VPK? CreateEmpty(string path, PackFileOptions options)
        {
            unsafe
            {
                var handle = Extern.vpkpp_vpk_create_empty_with_options(path, options);
                return handle == null ? null : new VPK(handle);
            }
        }

        public static VPK? CreateFromDirectory(string vpkPath, string contentPath, bool saveToDir = false)
        {
            unsafe
            {
                var handle = Extern.vpkpp_vpk_create_from_directory(vpkPath, contentPath, Convert.ToByte(saveToDir));
                return handle == null ? null : new VPK(handle);
            }
        }

        public static VPK? CreateFromDirectory(string vpkPath, string contentPath, PackFileOptions options, bool saveToDir = false)
        {
            unsafe
            {
                var handle = Extern.vpkpp_vpk_create_from_directory_with_options(vpkPath, contentPath, Convert.ToByte(saveToDir), options);
                return handle == null ? null : new VPK(handle);
            }
        }

        public new static VPK? Open(string path)
        {
            unsafe
            {
                var handle = Extern.vpkpp_vpk_open(path);
                return handle == null ? null : new VPK(handle);
            }
        }

        public new static VPK? Open(string path, EntryCallback callback)
        {
            unsafe
            {
                EntryCallbackNative callbackNative = (path, entry) =>
                {
                    callback(path, new Entry(entry, true));
                };
                var handle = Extern.vpkpp_vpk_open_with_callback(path, Marshal.GetFunctionPointerForDelegate(callbackNative));
                return handle == null ? null : new VPK(handle);
            }
        }

        public new static VPK? Open(string path, PackFileOptions options)
        {
            unsafe
            {
                var handle = Extern.vpkpp_vpk_open_with_options(path, options);
                return handle == null ? null : new VPK(handle);
            }
        }

        public new static VPK? Open(string path, PackFileOptions options, EntryCallback callback)
        {
            unsafe
            {
                EntryCallbackNative callbackNative = (path, entry) =>
                {
                    callback(path, new Entry(entry, true));
                };
                var handle = Extern.vpkpp_vpk_open_with_options_and_callback(path, options, Marshal.GetFunctionPointerForDelegate(callbackNative));
                return handle == null ? null : new VPK(handle);
            }
        }

        public static bool GenerateKeyPairFiles(string path)
        {
            unsafe
            {
                return Convert.ToBoolean(Extern.vpkpp_vpk_generate_keypair_files(path));
            }
        }

        public bool Sign(string filepath)
        {
            unsafe
            {
                return Convert.ToBoolean(Extern.vpkpp_vpk_sign_from_file(Handle, filepath));
            }
        }

        public bool Sign(byte[] privateKey, byte[] publicKey)
        {
            unsafe
            {
                fixed (byte* privateKeyBufferPtr = privateKey)
                {
                    fixed (byte* publicKeyBufferPtr = publicKey)
                    {
                        return Convert.ToBoolean(Extern.vpkpp_vpk_sign_from_mem(Handle, privateKeyBufferPtr, (ulong) privateKey.LongLength, publicKeyBufferPtr, (ulong) publicKey.LongLength));
                    }
                }
            }
        }

        public bool Sign(IEnumerable<byte> privateKey, IEnumerable<byte> publicKey)
        {
            var privateKeyData = privateKey.ToArray();
            var publicKeyData = publicKey.ToArray();
            unsafe
            {
                fixed (byte* privateKeyBufferPtr = privateKeyData)
                {
                    fixed (byte* publicKeyBufferPtr = publicKeyData)
                    {
                        return Convert.ToBoolean(Extern.vpkpp_vpk_sign_from_mem(Handle, privateKeyBufferPtr, (ulong) privateKeyData.LongLength, publicKeyBufferPtr, (ulong) publicKeyData.LongLength));
                    }
                }
            }
        }

        public uint Version
        {
            get
            {
                unsafe
                {
                    return Extern.vpkpp_vpk_get_version(Handle);
                }
            }
            set
            {
                unsafe
                {
                    Extern.vpkpp_vpk_set_version(Handle, Math.Clamp(value, 1, 2));
                }
            }
        }
    }
}
