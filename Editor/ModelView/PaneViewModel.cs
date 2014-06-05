using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ModelView
{
    public class PaneViewModel : ViewModelBase
    {
        public PaneViewModel()
        {
        }

        private string _title;
        public string Title
        {
            get { return _title; }
            set
            {
                if (value != _title)
                {
                    _title = value;
                    RaisePropertyChanged("Title");
                }
            }
        }

        private string _contentId;
        public string ContentId
        {
            get { return _contentId; }
            set
            {
                if (value != _contentId)
                {
                    _contentId = value;
                    RaisePropertyChanged("ContentId");
                }
            }
        }

        private bool _isSelected;
        public bool IsSelected
        {
            get { return _isSelected; }
            set
            {
                if (value != _isSelected)
                {
                    _isSelected = value;
                    RaisePropertyChanged("IsSelected");
                }
            }
        }

        private bool _isActive;
        public bool IsActive
        {
            get { return _isActive; }
            set
            {
                if (value != _isActive)
                {
                    _isActive = value;
                    RaisePropertyChanged("IsActive");
                }
            }
        }
    }
}
