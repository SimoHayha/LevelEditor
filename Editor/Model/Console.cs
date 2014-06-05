using System.ComponentModel;

namespace Editor.Model
{
    public class Console : INotifyPropertyChanged
    {
        private string _text;
        private ManagedLib.ManagedLog _managedLog;

        public string Text
        {
            get
            {
                return this._text;
            }
            set
            {
                this._text += value;
                NotifyPropertyChanged("Text");
            }
        }

        public ManagedLib.ManagedLog ManagedLog
        {
            get
            {
                return this._managedLog;
            }
            private set
            {
                this._managedLog = value;
                NotifyPropertyChanged("ManagedLog");
            }
        }
		
        public Console()
        {
           this.Init();
        }
		
        private void Init()
        {
			this.ManagedLog = new ManagedLib.ManagedLog();
            this.Text = "";
            this.ManagedLog.ValueChanged += this.ValueChangedHandler;
        }

        private void ValueChangedHandler(int i)
        {
            this.Text = this.ManagedLog.Value + "\n";
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
