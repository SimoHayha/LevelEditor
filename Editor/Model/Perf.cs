using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Model
{
    public class Perf : INotifyPropertyChanged
    {
        private int _primitives;
        private float _gpuUsage;
        private ManagedLib.ManagedPerf _managedPerf;

        public int Primitives
        {
            get
            {
                return _primitives;
            }
            set
            {
                if (value != _primitives)
                {
                    _primitives = value;
                    NotifyPropertyChanged("Primitives");
                }
            }
        }

        public float GpuUsage
        {
            get
            {
                return _gpuUsage;
            }
            set
            {
                if (value != _gpuUsage)
                {
                    _gpuUsage = value;
                    NotifyPropertyChanged("GpuUsage");
                }
            }
        }

        public ManagedLib.ManagedPerf ManagedPerf
        {
            get
            {
                return _managedPerf;
            }
            set
            {
                if (value != _managedPerf)
                {
                    _managedPerf = value;
                    NotifyPropertyChanged("ManagedPerf");
                }
            }
        }

        public Perf()
        {
            this.Init();
        }

        private void Init()
        {
            this.ManagedPerf = new ManagedLib.ManagedPerf();
            this.Primitives = 0;
            this.GpuUsage = 0.0f;
            this.ManagedPerf.ValueChanged += ManagedPerf_ValueChanged;
        }

        void ManagedPerf_ValueChanged(int i)
        {
            this.Primitives = ManagedPerf.Primitives;
            this.GpuUsage = ManagedPerf.GpuIdle;
        }

        private void NotifyPropertyChanged(string info)
        {
            if (null != PropertyChanged)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
