using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;

namespace Editor.ModelView
{
    public class MainWindowViewModel : INotifyPropertyChanged
    {
        #region data
        private Model.Console _console;
        private Model.Model model_;
        #endregion

        #region relay
        private ICommand menuNewCommand;
        private ICommand menuOpenCommand;
        private ICommand menuSaveCommand;
        private ICommand menuExitCommand;
        private ICommand menuAddCommand;
        private ICommand treeviewSelectedItemChangedCommand;
        private ICommand animationChangedCommand;
        private ICommand viewportSelectionCommand;
        private ICommand menuAddViewportCommand;
        private ICommand menuRemoveViewportCommand;

        public ICommand MenuNewCommand
        {
            get { return this.menuNewCommand; }
            set { this.menuNewCommand = value; }
        }

        public ICommand MenuOpenCommand
        {
            get { return this.menuOpenCommand; }
            set { this.menuOpenCommand = value; }
        }

        public ICommand MenuSaveCommand
        {
            get { return this.menuSaveCommand; }
            set { this.menuSaveCommand = value; }
        }

        public ICommand MenuExitCommand
        {
            get { return this.menuExitCommand; }
            set { this.menuExitCommand = value; }
        }

        public ICommand MenuAddCommand
        {
            get { return this.menuAddCommand; }
            set { this.menuAddCommand = value; }
        }

        public ICommand TreeViewSelectedItemChangedCommand
        {
            get { return this.treeviewSelectedItemChangedCommand; }
            set { this.treeviewSelectedItemChangedCommand = value; }
        }

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

        public ICommand MenuAddViewportCommand
        {
            get { return this.menuAddViewportCommand; }
            set { this.menuAddViewportCommand = value; }
        }

        public ICommand MenuRemoveViewportCommand
        {
            get { return this.menuRemoveViewportCommand; }
            set { this.menuRemoveViewportCommand = value; }
        }
        #endregion

        public Model.Console Console
        {
            get
            {
                return this._console;
            }
            set
            {
                this._console = value;
                NotifyPropertyChanged("Console");
            }
        }

        public Model.Model Model
        {
            get
            {
                return this.model_;
            }
            set
            {
                this.model_ = value;
                NotifyPropertyChanged("Model");
            }
        }

        public MainWindowViewModel()
        {
            this.BuildRelay();
        }

        public void OnLoad(WpfHostingWin32Control.ControlHost controlHost)
        {
            this.Model = new Model.Model(controlHost);
            this.Console = new Model.Console();

            //this.Model.OnLoad(controlHost);
        }

        private void BuildRelay()
        {
            this.MenuNewCommand = new RelayCommand(this.MenuNewExecute);
            this.MenuOpenCommand = new RelayCommand(this.MenuOpenExecute);
            this.MenuSaveCommand = new RelayCommand(this.MenuSaveExecute);
            this.MenuExitCommand = new RelayCommand(this.MenuExitExecute);
            this.MenuAddCommand = new RelayCommand(this.MenuAddExecute);
            this.TreeViewSelectedItemChangedCommand = new RelayCommand(this.TreeViewSelectedItemChangedExecute);
            this.AnimationChangedCommand = new RelayCommand(this.AnimationChangedExecute);
            this.ViewportSelectionCommand = new RelayCommand(this.ViewportSelectionExecute);
            this.menuAddViewportCommand = new RelayCommand(this.MenuAddViewportExecute);
            this.menuRemoveViewportCommand = new RelayCommand(this.MenuRemoveViewportExecute);
        }

        #region event handler
        public void MenuNewExecute(object obj)
        {
        }

        public void MenuOpenExecute(object obj)
        {
        }

        public void MenuSaveExecute(object obj)
        {
        }

        public void MenuExitExecute(object obj)
        {
        }

        public void MenuAddExecute(object obj)
        {
            this.AddModel(Convert.ToInt32(obj));
        }

        public void TreeViewSelectedItemChangedExecute(object obj)
        {
            this.model_.SelectedEntity = obj as ManagedLib.ManagedEntity;
        }

        public void AnimationChangedExecute(object obj)
        {
            this.AnimationChanged(Convert.ToInt32(obj));
        }

        public void ViewportSelectionExecute(object obj)
        {
            this.ViewportSelection(Convert.ToInt32(obj));
        }

        public void MenuAddViewportExecute(object obj)
        {
            this.ViewportAdd();
        }

        public void MenuRemoveViewportExecute(object obj)
        {
            this.ViewportRemove();
        }
        #endregion

        private void AddModel(int type)
        {
            string name = null;

            if (0 == type)
            {
                OpenFileDialog dial = new OpenFileDialog();

                dial.Filter = "FBX files (*.FBX)|*.fbx";
                dial.Title = "Select a FBX File";

                if (System.Windows.Forms.DialogResult.OK == dial.ShowDialog())
                    name = dial.FileName;
            }
            this.model_.AddModel(type, name);
        }

        private void AnimationChanged(int index)
        {
            this.model_.PerformAnimationChanged(index);
        }

        private void ViewportSelection(int index)
        {
            this.model_.PerformViewportSelection(index);
        }

        private void ViewportAdd()
        {
            this.model_.ViewportAdd();
        }

        private void ViewportRemove()
        {
            this.model_.ViewportRemove();
			var newWindow = new RessourcesWindow();
			newWindow.Show();
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
