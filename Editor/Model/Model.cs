using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Model
{
    public class Model : INotifyPropertyChanged
    {
        private System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity> managedEntities_ = null;
        private String debugString_ = null;
        private ManagedLib.ManagedEntity selectedEntity = null;
        private WpfHostingWin32Control.ControlHost controlHost = null;


        public System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity> Entities
        {
            get
            {
                return this.managedEntities_;
            }
            set
            {
                this.managedEntities_ = value;
                NotifyPropertyChanged("Entities");
            }
        }

        public String DebugString
        {
            get
            {
                return this.debugString_;
            }
            set
            {
                this.debugString_ = value;
                NotifyPropertyChanged("DebugString");
            }
        }

        public ManagedLib.ManagedEntity SelectedEntity
        {
            get
            {
                return this.selectedEntity;
            }
            set
            {
                this.selectedEntity = value;
                NotifyPropertyChanged("SelectedEntity");
            }
        }

        public Model(WpfHostingWin32Control.ControlHost controlHost)
        {
            this.controlHost = controlHost;
        }

		
        public void AddModel(int type, string path)
        {
            if (IntPtr.Zero == this.controlHost.addEntity(type, this.selectedEntity, path))
                System.Console.WriteLine("Add entity problem");
            this.Entities = this.controlHost.getEntities();
        }

        public void PerformAnimationChanged(int index)
        {
            if (null == this.SelectedEntity)
                return;

            this.SelectedEntity.CurrentAnimation = index;
            NotifyPropertyChanged("CurrentAnimation");
        }

        public void PerformViewportSelection(int index)
        {
            if (null == this.SelectedEntity || 6 != this.SelectedEntity.Type)
                return;

            this.controlHost.SetCamera(this.SelectedEntity, index);
        }

        public void ViewportAdd()
        {
            this.controlHost.ViewportChange(1);
        }

        public void ViewportRemove()
        {
            this.controlHost.ViewportChange(-1);
        }

        private void NotifyPropertyChanged(string info)
        {
            if (null != PropertyChanged)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }

		public ManagedLib.Managed GetManaged()
		{
			return controlHost.m;
		}

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
