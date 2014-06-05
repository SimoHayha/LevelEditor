using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Editor.ModelView
{
	public class ViewPortViewModel : ToolViewModel
	{
		private WpfHostingWin32Control.ControlHost controlHost = null;

		private ManagedLib.ManagedEntity selectedEntity = null;
		public ManagedLib.ManagedEntity SelectedEntity
		{
			get
			{
				return this.selectedEntity;
			}
			set
			{
				this.selectedEntity = value;
				RaisePropertyChanged("SelectedEntity");
			}
		}

		private System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity> _cameras;
		public System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity> Cameras
		{
			get { return _cameras; }
			set
			{
				if (value != _cameras)
				{
					_cameras = value;
					RaisePropertyChanged("Cameras");
				}
			}
		}

		private int[] _indices = new int[4];

		public int IndexViewport0
		{
			get { return _indices[0]; }
			set
			{
				if (value != _indices[0])
				{
					_indices[0] = value;
					RaisePropertyChanged("IndexViewport0");
				}
			}
		}

		public int IndexViewport1
		{
			get { return _indices[1]; }
			set
			{
				if (value != _indices[1])
				{
					_indices[1] = value;
					RaisePropertyChanged("IndexViewport1");
				}
			}
		}

		public int IndexViewport2
		{
			get { return _indices[2]; }
			set
			{
				if (value != _indices[2])
				{
					_indices[2] = value;
					RaisePropertyChanged("IndexViewport2");
				}
			}
		}

		public int IndexViewport3
		{
			get { return _indices[3]; }
			set
			{
				if (value != _indices[3])
				{
					_indices[3] = value;
					RaisePropertyChanged("IndexViewport3");
				}
			}
		}

		private bool[] needChange = {true , true, true, true};

		private ICommand viewportSelectionCommand0;
		public ICommand ViewportSelectionCommand0
		{
			get { return this.viewportSelectionCommand0; }
			set { this.viewportSelectionCommand0 = value; }
		}
		public void ViewportSelectionExecute0(object obj)
		{
			if (!needChange[0])
			{
				needChange[0] = !needChange[0];
				return;
			}
			int index = Convert.ToInt32(obj);
			int oldViewport = Cameras[index].ViewPortID;
			this.controlHost.SetCamera(Cameras[index], 1);
			resetComboBoxes(oldViewport, Cameras[index]);
		}

		private ICommand viewportSelectionCommand1;
		public ICommand ViewportSelectionCommand1
		{
			get { return this.viewportSelectionCommand1; }
			set { this.viewportSelectionCommand1 = value; }
		}
		public void ViewportSelectionExecute1(object obj)
		{
			if (!needChange[1])
			{
				needChange[1] = !needChange[1];
				return;
			}
			int index = Convert.ToInt32(obj);
			int oldViewport = Cameras[index].ViewPortID;
			this.controlHost.SetCamera(Cameras[index], 2);
			resetComboBoxes(oldViewport, Cameras[index]);
		}

		private ICommand viewportSelectionCommand2;
		public ICommand ViewportSelectionCommand2
		{
			get { return this.viewportSelectionCommand2; }
			set { this.viewportSelectionCommand2 = value; }
		}
		public void ViewportSelectionExecute2(object obj)
		{
			if (!needChange[2])
			{
				needChange[2] = !needChange[2];
				return;
			}
			int index = Convert.ToInt32(obj);
			int oldViewport = Cameras[index].ViewPortID;
			this.controlHost.SetCamera(Cameras[index], 3);
			resetComboBoxes(oldViewport, Cameras[index]);
		}

		private ICommand viewportSelectionCommand3;
		public ICommand ViewportSelectionCommand3
		{
			get { return this.viewportSelectionCommand3; }
			set { this.viewportSelectionCommand3 = value; }
		}
		public void ViewportSelectionExecute3(object obj)
		{
			if (!needChange[3])
			{
				needChange[3] = !needChange[3];
				return;
			}
			int index = Convert.ToInt32(obj);
			int oldViewport = Cameras[index].ViewPortID;
			this.controlHost.SetCamera(Cameras[index], 4);
			resetComboBoxes(oldViewport, Cameras[index]);
		}

		public ViewPortViewModel() :
			base("ViewPort")
		{
			ContentId = ToolContentId;
		}

		public const string ToolContentId = "ViewPortViewTool";

		internal void OnLoad(WpfHostingWin32Control.ControlHost controlHost)
		{
            if (null == controlHost)
                return;

			this.controlHost = controlHost;
			this.Cameras = new System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity>();
			for (int i = 0; i < 4; ++i)
			{
                //if (false == this.controlHost.AddCameraEditor())
                //    return;
				if (IntPtr.Zero == this.controlHost.m.addCameraEditor())
					System.Console.WriteLine("Add entity problem");
				ManagedLib.ManagedEntity entity = new ManagedLib.ManagedEntity();

				this.controlHost.m.setEntityEditor(entity, i);
				this.Cameras.Add(entity);
				this.controlHost.SetCamera(entity, i + 1);
			}
			this.ViewportSelectionCommand0 = new RelayCommand(this.ViewportSelectionExecute0);
			this.ViewportSelectionCommand1 = new RelayCommand(this.ViewportSelectionExecute1);
			this.ViewportSelectionCommand2 = new RelayCommand(this.ViewportSelectionExecute2);
			this.ViewportSelectionCommand3 = new RelayCommand(this.ViewportSelectionExecute3);
			IndexViewport0 = 0;
			IndexViewport1 = 1;
			IndexViewport2 = 2;
			IndexViewport3 = 3;
		}

		private void resetComboBoxes(int oldViewport, ManagedLib.ManagedEntity entity)
		{
			if (oldViewport == entity.ViewPortID || oldViewport == 0)
				return;
			needChange[oldViewport - 1] = false;
			if (oldViewport != -1)
				_indices[oldViewport - 1] = -1;
			if (oldViewport == 1)
				RaisePropertyChanged("IndexViewport0");
			else if (oldViewport == 2)
				RaisePropertyChanged("IndexViewport1");
			else if (oldViewport == 3)
				RaisePropertyChanged("IndexViewport2");
			else if (oldViewport == 4)
				RaisePropertyChanged("IndexViewport3");
		}

        private bool cam1Wire_ = false;
        public bool Cam1Wire
        {
            get
            {
                return cam1Wire_;
            }
            set
            {
                if (value != cam1Wire_)
                {
                    cam1Wire_ = value;

                    this.controlHost.Wireframe(0, value);
                }
            }
        }

        private bool cam2Wire_ = false;
        public bool Cam2Wire
        {
            get
            {
                return cam2Wire_;
            }
            set
            {
                if (value != cam2Wire_)
                {
                    cam2Wire_ = value;

                    this.controlHost.Wireframe(1, value);
                }
            }
        }

        private bool cam3Wire_ = false;
        public bool Cam3Wire
        {
            get
            {
                return cam3Wire_;
            }
            set
            {
                if (value != cam3Wire_)
                {
                    cam3Wire_ = value;

                    this.controlHost.Wireframe(2, value);
                }
            }
        }

        private bool cam4Wire_ = false;
        public bool Cam4Wire
        {
            get
            {
                return cam4Wire_;
            }
            set
            {
                if (value != cam4Wire_)
                {
                    cam4Wire_ = value;

                    this.controlHost.Wireframe(3, value);
                }
            }
        }
    }
}
