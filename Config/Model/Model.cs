using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Config.Model
{
    public class Model : INotifyPropertyChanged
    {
        private readonly string FILENAME = "config.xml";
        private string errString_ = null;

        public string ErrString
        {
            get
            {
                return this.errString_;
            }
            private set
            {
                this.errString_ = value;
                NotifyPropertyChanged("ErrString");
            }
        }

        public RelayCommand OpenFileCommand
        {
            get;
            set;
        }

        public Model()
        {
            this.ErrString = "OK";

            this.OpenFileCommand = new RelayCommand(this.OpenFileCallback);
        }

        public void OpenFileCallback(Object o)
        {
            Console.WriteLine("LOL");
        }

        public bool Load()
        {
            XDocument xdoc = null;

            try
            {
                xdoc = XDocument.Load(this.FILENAME);
            }
            catch (ArgumentNullException e)
            {
                this.ErrString = e.Message;
                return false;
            }
            catch (SecurityException e)
            {
                this.ErrString = e.Message;
                return false;
            }
            catch (FileNotFoundException e)
            {
                this.ErrString = e.Message;
                return false;
            }
            catch (UriFormatException e)
            {
                this.ErrString = e.Message;
                return false;
            }

            var request = xdoc.Descendants("Configuration");

            foreach (var result in request)
            {
            }

            return true;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void NotifyPropertyChanged(string info)
        {
            if (null != PropertyChanged)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }
    }
}
