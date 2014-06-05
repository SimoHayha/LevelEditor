#region Using directives

using System;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;
using System.Windows.Interop;

#endregion

namespace WpfHostingWin32Control
{
    public class ControlHost : HwndHost
    {
        IntPtr hwndControl;
        IntPtr hwndHost;
        double hostHeight, hostWidth;
        public HandleRef HwndParent;
        public ManagedLib.Managed m;
        private Editor.MainWindow mainWindow;
        bool errorOccured;

        public ControlHost(double height, double width)
        {
            hostHeight = height;
            hostWidth = width;
            errorOccured = false;
        }

        public ControlHost(double height, double width, Editor.MainWindow mainWindow)
        {
            hostHeight = height;
            hostWidth = width;
            errorOccured = false;
            this.mainWindow = mainWindow;
        }

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            hwndControl = IntPtr.Zero;
            hwndHost = IntPtr.Zero;
            hwndHost = CreateWindowEx(0, "static", "",
                                      WS_CHILD | WS_VISIBLE,
                                      100, 100,
                                      (int)hostWidth, (int)hostHeight,
                                      hwndParent.Handle,
                                      (IntPtr)HOST_ID,
                                      IntPtr.Zero,
                                      0);
            m = new ManagedLib.Managed();
            hwndControl = m.init(hwndHost, (uint)hostWidth, (uint)hostHeight);
            if (IntPtr.Zero == hwndControl)
            {
                errorOccured = true;
                MessageBox.Show("Cannot initialize DirectX engine");
                this.mainWindow.Close();
                return new HandleRef(null, IntPtr.Zero);
            }
            m.ValueChanged += MySetFocus;
            return new HandleRef(this, hwndHost);
        }

        public void render()
        {
            if (null != m && !errorOccured)
                m.run(hwndControl);
        }

        protected override void OnRender(System.Windows.Media.DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);
        }

	    public IntPtr addEntity(int type, ManagedLib.ManagedEntity parent = null, string path = null)
	    {
            if (errorOccured)
                return IntPtr.Zero;

	        return m.addEntity(type, parent, path);
	    }

	    public IntPtr addEntity(int type, IntPtr parent, string path = null)
	    {
            if (errorOccured)
                return IntPtr.Zero;

	    	return m.addEntity(type, parent, path);
	    }

	    public IntPtr addEntityToChildModel(int type, IntPtr model, uint index, string path = null)
	    {
            if (errorOccured)
                return IntPtr.Zero;

	    	return m.addEntityToChildModel(type, model, index, path);
	    }

		public void setEntity(ManagedLib.ManagedEntity entity, int index, ManagedLib.ManagedEntity parent = null)
		{
            if (errorOccured)
                return;

			m.setEntity(entity, index, parent);
		}

        public void resetScene()
        {
            if (errorOccured)
                return;

            m.resetScene();
        }

        public ObservableCollection<ManagedLib.ManagedEntity> getEntities()
        {
            if (errorOccured)
                return null;

            if (null == m)
                return null;
            return m.getEntities();
        }

        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            handled = false;
            return IntPtr.Zero;
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            DestroyWindow(hwnd.Handle);
        }

        public IntPtr hwndListBox
        {
            get { return hwndControl; }
        }

        public void MySetFocus()
        {
            SetFocus(this.hwndControl);
        }

		public void SetCamera(ManagedLib.ManagedEntity entity, int viewport)
		{
            if (errorOccured)
                return;

			m.setCamera(entity, viewport);
		}

		public void ViewportChange(int diff)
		{
            if (errorOccured)
                return;

			m.viewportChange(diff);
		}

		public void Resize(int height, int width)
		{
            if (errorOccured)
                return;

			m.resize(height, width);
		}

        public void unselect()
        {
            if (errorOccured)
                return;

            m.unselect();
        }

        public void ActionChanged(string action)
        {
            if (errorOccured)
                return;

            m.actionChanged(action);
        }

        public void Wireframe(int cam, bool wireframe)
        {
            if (errorOccured)
                return;

            m.wireframe(cam, wireframe);
        }

        public bool AddCameraEditor()
        {
            if (errorOccured)
                return false;

            if (IntPtr.Zero == m.addCameraEditor())
                return false;

            return true;
        }

        public void LoadVmf(string name)
        {
            if (errorOccured)
                return;

            m.loadVmf(name);
        }

        //PInvoke declarations
        [DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
        internal static extern IntPtr CreateWindowEx(int dwExStyle,
                                                      string lpszClassName,
                                                      string lpszWindowName,
                                                      int style,
                                                      int x, int y,
                                                      int width, int height,
                                                      IntPtr hwndParent,
                                                      IntPtr hMenu,
                                                      IntPtr hInst,
                                                      [MarshalAs(UnmanagedType.AsAny)] object pvParam);
        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);
        internal const int
          WS_CHILD = 0x40000000,
          WS_VISIBLE = 0x10000000,
          LBS_NOTIFY = 0x00000001,
          HOST_ID = 0x00000002,
          LISTBOX_ID = 0x00000001,
          WS_VSCROLL = 0x00200000,
          WS_BORDER = 0x00800000;

        [DllImport("user32.dll", EntryPoint = "SendMessage", CharSet = CharSet.Unicode)]
        internal static extern int SendMessage(IntPtr hwnd,
                                               int msg,
                                               IntPtr wParam,
                                               IntPtr lParam);

        [DllImport("user32.dll", EntryPoint = "SendMessage", CharSet = CharSet.Unicode)]
        internal static extern int SendMessage(IntPtr hwnd,
                                               int msg,
                                               int wParam,
                                               [MarshalAs(UnmanagedType.LPWStr)] StringBuilder lParam);

        [DllImport("user32.dll", EntryPoint = "SendMessage", CharSet = CharSet.Unicode)]
        internal static extern IntPtr SendMessage(IntPtr hwnd,
                                                  int msg,
                                                  IntPtr wParam,
                                                  String lParam);

        [DllImport("user32.dll", EntryPoint = "SetFocus", CharSet = CharSet.Unicode)]
        internal static extern IntPtr SetFocus(IntPtr hwnd);
    }
}
