using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Editor.ModelView
{
    public class PropertyViewModel : ToolViewModel
    {
        
		public ManagedLib.ManagedRessources Ressources { get; set; }

        public PropertyViewModel() :
            base("Property")
        {
            ContentId = ToolContentId;

            this.AnimationChangedCommand = new RelayCommand(this.AnimationChangedExecute);
            this.ViewportSelectionCommand = new RelayCommand(this.ViewportSelectionExecute);
            this.FeedbackControlCommand = new RelayCommand(this.FeedbackControlExecute);
            this.ScriptActionCommand = new RelayCommand(this.ScriptActionExecute);
        }

        public const string ToolContentId = "PropertyViewTool";

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
                if (null == this.SelectedEntity)
                    this.Active = false;
                else
                    this.Active = true;
                RaisePropertyChanged("SelectedEntity");
            }
        }

        private ICommand animationChangedCommand;
        private ICommand viewportSelectionCommand;
        private ICommand feedbackControlCommand;
        private ICommand scriptActionCommand;

        public ICommand AnimationChangedCommand
        {
            get { return this.animationChangedCommand; }
            set { this.animationChangedCommand = value; }
        }

        public ICommand ViewportSelectionCommand
        {
            get { return this.viewportSelectionCommand; }
            set { this.viewportSelectionCommand = value; }
        }

        public ICommand FeedbackControlCommand
        {
            get { return this.feedbackControlCommand; }
            set { this.feedbackControlCommand = value; }
        }

        public ICommand ScriptActionCommand
        {
            get { return this.scriptActionCommand; }
            set { this.scriptActionCommand = value; }
        }

        public void AnimationChangedExecute(object obj)
        {
            int index = Convert.ToInt32(obj);
            
            if (null == this.SelectedEntity)
                return;

            this.SelectedEntity.CurrentAnimation = index;
            RaisePropertyChanged("CurrentAnimation");
        }

        public void ScriptActionExecute(object obj)
        {
            string command = obj as string;

            if ("Play" == command)
                this.SelectedEntity.ScriptRunning = true;
            else if ("Pause" == command)
                this.SelectedEntity.ScriptRunning = false;
        }

        private WpfHostingWin32Control.ControlHost controlHost = null;

		private ManagedLib.ManagedEntity selecE = null;

        public void ViewportSelectionExecute(object obj)
        {
			if (this.SelectedEntity != this.selecE)
			{
				selecE = this.SelectedEntity;
				return;
			}
            int index = Convert.ToInt32(obj);
            this.controlHost.SetCamera(this.SelectedEntity, index);
        }

        public void FeedbackControlExecute(object obj)
        {
        }

        public void OnLoad(WpfHostingWin32Control.ControlHost controlHost, ManagedLib.ManagedRessources ressources)
        {
            this.controlHost = controlHost;
            this.Ressources = ressources;
        }

        private bool _active = false;
        public bool Active
        {
            get
            {
                return _active;
            }
            private set
            {
                if (value != this._active)
                {
                    this._active = value;
                    RaisePropertyChanged("Active");
                }
            }
        }
    }
}
