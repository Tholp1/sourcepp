using System;
using System.Runtime.InteropServices;

namespace vpkpp.Format
{
    using EntryCallback = Action<string, Entry>;

    internal static unsafe partial class Extern
    {
        [DllImport("vpkppc")]
        public static extern void* vpkpp_gcf_open([MarshalAs(UnmanagedType.LPStr)] string path);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_gcf_open_with_callback([MarshalAs(UnmanagedType.LPStr)] string path, IntPtr callback);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_gcf_open_with_options([MarshalAs(UnmanagedType.LPStr)] string path, PackFileOptions options);

        [DllImport("vpkppc")]
        public static extern void* vpkpp_gcf_open_with_options_and_callback([MarshalAs(UnmanagedType.LPStr)] string path, PackFileOptions options, IntPtr callback);
    }

    public class GCF : PackFile
    {
        private protected unsafe GCF(void* handle) : base(handle) {}

        public new static GCF? Open(string path)
        {
            unsafe
            {
                var handle = Extern.vpkpp_gcf_open(path);
                return handle == null ? null : new GCF(handle);
            }
        }

        public new static GCF? Open(string path, EntryCallback callback)
        {
            unsafe
            {
                EntryCallbackNative callbackNative = (path, entry) =>
                {
                    callback(path, new Entry(entry, true));
                };
                var handle = Extern.vpkpp_gcf_open_with_callback(path, Marshal.GetFunctionPointerForDelegate(callbackNative));
                return handle == null ? null : new GCF(handle);
            }
        }

        public new static GCF? Open(string path, PackFileOptions options)
        {
            unsafe
            {
                var handle = Extern.vpkpp_gcf_open_with_options(path, options);
                return handle == null ? null : new GCF(handle);
            }
        }

        public new static GCF? Open(string path, PackFileOptions options, EntryCallback callback)
        {
            unsafe
            {
                EntryCallbackNative callbackNative = (path, entry) =>
                {
                    callback(path, new Entry(entry, true));
                };
                var handle = Extern.vpkpp_gcf_open_with_options_and_callback(path, options, Marshal.GetFunctionPointerForDelegate(callbackNative));
                return handle == null ? null : new GCF(handle);
            }
        }
    }
}
