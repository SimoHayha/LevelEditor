using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Editor.ModelView
{
    public class AnimationViewModel : ToolViewModel
    {
        public AnimationViewModel() :
            base("Animation")
        {
            ContentId = ToolContentId;

            this.AnimationChangedCommand = new RelayCommand(this.AnimationChangedExecute);
            this.AnimationActionCommand = new RelayCommand(this.AnimationActionExecute);
        }

        public const string ToolContentId = "AnimationTool";

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
                if (this.selectedEntity != null)
                {
                    if (this.selectedEntity.AnimationNames.Count > 0)
                    {
                        this.Active = true;
                        this.ErrMsg = null;
                        this.SelectedEntity.CurrentAnimation = 0;
                    }
                    else
                    {
                        this.Active = false;
                        this.ErrMsg = "This object doesn't have any usable animation";
                    }
                }
                else
                {
                    this.Active = false;
                    this.ErrMsg = "No entity selected";
                }
                RaisePropertyChanged("SelectedEntity");
            }
        }

        private string _errMsg = "No entity selected";
        public string ErrMsg
        {
            get
            {
                return this._errMsg;
            }
            private set
            {
                this._errMsg = value;
                RaisePropertyChanged("ErrMsg");
            }
        }

        private bool _active;
        public bool Active
        {
            get
            {
                return this._active;
            }
            private set
            {
                this._active = value;
                RaisePropertyChanged("Active");
            }
        }

        private ICommand animationChangedCommand;
        private ICommand animationActionCommand;

        public ICommand AnimationChangedCommand
        {
            get { return this.animationChangedCommand; }
            set { this.animationChangedCommand = value; }
        }

        public ICommand AnimationActionCommand
        {
            get { return this.animationActionCommand; }
            set { this.animationActionCommand = value; }
        }

        public void AnimationChangedExecute(object obj)
        {
            int index = Convert.ToInt32(obj);

            if (null == this.SelectedEntity)
                return;

            this.SelectedEntity.CurrentAnimation = index;
            RaisePropertyChanged("CurrentAnimation");
        }

        public void AnimationActionExecute(object obj)
        {
            string command = obj as string;

            if (null == command)
                return;

            if (command == "Restart")
            {
                this.SelectedEntity.CurrentTime = 0.0;
                this.SelectedEntity.Animate = true;
            }
            else if (command == "Play")
                this.SelectedEntity.Animate = true;
            else if (command == "Pause")
                this.SelectedEntity.Animate = false;
        }
    }
}
