using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Editor.ModelView
{
    public class TreeViewModel : ToolViewModel
    {
        public TreeViewModel() :
            base("Entity")
        {
            ContentId = ToolContentId;

            this.TreeViewSelectedItemChangedCommand = new RelayCommand(this.TreeViewSelectedItemChangedExecute);
            this.TreeViewClickCommand = new RelayCommand(this.TreeViewClickExecute);
        }

        public const string ToolContentId = "TreeViewTool";
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
                this._selectedEntityChangedAction(value);
                RaisePropertyChanged("SelectedEntity");
            }
        }

        private System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity> _entities;
        public System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity> Entities
        {
            get { return _entities; }
            set
            {
                if (value != _entities)
                {
                    _entities = value;
                    RaisePropertyChanged("Entities");
                }
            }
        }

        private System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity> _fastDP;
        public System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity> FastDP
        {
            get { return _fastDP; }
            set
            {
                if (value != _fastDP)
                {
                    _fastDP = value;
                    RaisePropertyChanged("FastDP");
                }
            }
        }

        private Action<ManagedLib.ManagedEntity> _selectedEntityChangedAction;

        public void OnLoad(WpfHostingWin32Control.ControlHost controlHost, Action<ManagedLib.ManagedEntity> action)
        {
            this.controlHost = controlHost;
            this._selectedEntityChangedAction = action;

            if (controlHost != null)
            {
                this.Entities = controlHost.getEntities();
                if (this.Entities != null)
                    this.SetCallbackEntities(this.Entities);
            }
        }

        public void SetCallbackEntities(System.Collections.ObjectModel.ObservableCollection<ManagedLib.ManagedEntity> entities)
        {
            foreach (var it in entities)
            {
                it.ValueChanged += this.SelectionDXChanged;

                this.SetCallbackEntities(it.Childs);
            }
        }

        public void SelectionDXChanged(ManagedLib.ManagedEntity sel)
        {
            this.SelectedEntity = sel;
        }

        public void OnAddModel(int type, string path)
        {
            ManagedLib.ManagedEntity newEntity = null;
            if (path == null && type != 0)
            {
                if (IntPtr.Zero == this.controlHost.addEntity(type, this.selectedEntity, path))
                    System.Console.WriteLine("Add entity problem");
                newEntity = new ManagedLib.ManagedEntity();
                newEntity.ValueChanged += this.SelectionDXChanged;
                if (this.selectedEntity != null)
                {
                    this.controlHost.setEntity(newEntity, selectedEntity.Childs.Count(), this.SelectedEntity);
                    this.selectedEntity.Childs.Add(newEntity);
                }
                else
                {
                    this.controlHost.setEntity(newEntity, Entities.Count);
                    this.Entities.Add(newEntity);
                }
                newEntity.setMaterialList();
            }
            else if (path != null)
            {
                AsyncAddFbx(type, path);
            }
        }

        public async void AsyncAddFbx(int type, string path)
        {
            bool ret;

            await Task<bool>.Run(new Action(() => ret = this.AddModel(type, path)));
            this.Entities = controlHost.getEntities();
            this.SetCallbackEntities(this.Entities);
        }

        private bool AddModel(int type, string path)
        {
            if (IntPtr.Zero == this.controlHost.addEntity(type, this.selectedEntity, path))
            {
                System.Console.WriteLine("Add entity problem");
                return false;
            }
            return true;
        }

        private ICommand treeviewSelectedItemChangedCommand;
        private ICommand treeviewClickCommand;

        public ICommand TreeViewSelectedItemChangedCommand
        {
            get { return this.treeviewSelectedItemChangedCommand; }
            set { this.treeviewSelectedItemChangedCommand = value; }
        }

        public ICommand TreeViewClickCommand
        {
            get { return this.treeviewClickCommand; }
            set { this.treeviewClickCommand = value; }
        }

        private void ClearTreeViewControlSelection(ItemCollection ic, ItemContainerGenerator icg)
        {
            if ((ic != null) && (icg != null))
            {
                for (var i = 0; i < ic.Count; ++i)
                {
                    TreeViewItem tvi = icg.ContainerFromIndex(i) as TreeViewItem;
                    if (null != tvi)
                    {
                        ClearTreeViewControlSelection(tvi.Items, tvi.ItemContainerGenerator);
                        tvi.IsSelected = false;
                    }
                }
            }
        }

        public void TreeViewSelectedItemChangedExecute(object obj)
        {
            if (null == obj)
                return;

            TreeView treeView = obj as TreeView;

            if (null == treeView)
                return;

            this.SelectedEntity = treeView.SelectedItem as ManagedLib.ManagedEntity;
        }

        public void TreeViewClickExecute(object obj)
        {
            if (null == obj)
                return;

            TreeView treeView = obj as TreeView;

            if (null == treeView)
                return;

            ItemCollection ic = treeView.Items;
            ItemContainerGenerator icg = treeView.ItemContainerGenerator;

            this.ClearTreeViewControlSelection(ic, icg);
            this.SelectedEntity = null;
            this.controlHost.unselect();
        }

        public void OptionChanged(string action)
        {
            this.controlHost.ActionChanged(action);
        }

        private System.Windows.Point _lastMouseDown;
        private TreeViewItem draggedItem;

        private void TreeView_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                _lastMouseDown = e.GetPosition(sender as System.Windows.IInputElement);
            }
        }

        private void TreeView_MouseMove(object sender, MouseEventArgs e)
        {
            //try
            //{
            //    if (e.LeftButton == MouseButtonState.Pressed)
            //    {
            //        Point currentPosition = e.GetPosition(sender as System.Windows.IInputElement);

            //        if ((Math.Abs(currentPosition.X - _lastMouseDown.X) > 10.0) ||
            //            (Math.Abs(currentPosition.Y - _lastMouseDown.Y) > 10.0))
            //        {
            //            draggedItem = (TreeViewItem)(sender as TreeView).SelectedItem;

            //            if (draggedItem != null)
            //            {
            //                DragDropEffects finalDropEffect = DragDrop.DoDragDrop((sender as TreeView), (sender as TreeView).SelectedValue, DragDropEffects.Move);

            //                if ((finalDropEffect == DragDropEffects.Move) && (_target != null))
            //                {
            //                    if (!draggedItem.Header.ToString().Equals(
            //                }
            //            }
            //        }
            //    }
            //}
        }
    }
}
