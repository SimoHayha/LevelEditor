using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using System.Xml.Serialization;

namespace Editor.ModelView
{
    public class Workspace : ViewModelBase
    {
        private string resourcePath = Directory.GetCurrentDirectory() + "\\Resources";

		private ManagedLib.ManagedRessources ressources = null;
		private XmlSerializer SerializerObj;
		private WpfHostingWin32Control.ControlHost control = null; 

        protected Workspace()
        {
			SerializerObj = new XmlSerializer(typeof(ObservableCollection<SerializableEntity>));
            this.BuildRelay();
            this.Autoload = false;

            if (!Directory.Exists(this.resourcePath))
                Directory.CreateDirectory(resourcePath);
        }

        static Workspace _this = new Workspace();

        public static Workspace This
        {
            get { return _this; }
        }

        ToolViewModel[] _tools = null;

        public IEnumerable<ToolViewModel> Tools
        {
            get
            {
                if (null == _tools)
					_tools = new ToolViewModel[] { TreeViewModel, PropertyViewModel, AnimationViewModel, ConsoleViewModel, MaterialViewModel, TextureViewModel, PerfViewModel, ScriptViewModel, ViewPortViewModel, ShaderViewModel};
                return _tools;
            }
        }

        TreeViewModel _treeViewModel = null;
        public TreeViewModel TreeViewModel
        {
            get
            {
                if (null == _treeViewModel)
                    _treeViewModel = new TreeViewModel();

                return _treeViewModel;
            }
        }

        PropertyViewModel _propertyViewModel = null;
        public PropertyViewModel PropertyViewModel
        {
            get
            {
                if (null == _propertyViewModel)
                    _propertyViewModel = new PropertyViewModel();

                return _propertyViewModel;
            }
        }

        AnimationViewModel _animationViewModel = null;
        public AnimationViewModel AnimationViewModel
        {
            get
            {
                if (null == _animationViewModel)
                    _animationViewModel = new AnimationViewModel();

                return _animationViewModel;
            }
        }

        ConsoleViewModel _consoleViewModel = null;
        public ConsoleViewModel ConsoleViewModel
        {
            get
            {
                if (null == _consoleViewModel)
                    _consoleViewModel = new ConsoleViewModel();

                return _consoleViewModel;
            }
        }

		MaterialViewModel _materialViewModel = null;
		public MaterialViewModel MaterialViewModel
		{
			get
			{
				if (null == _materialViewModel)
					_materialViewModel = new MaterialViewModel();

				return _materialViewModel;
			}
		}

		TextureViewModel _textureViewModel = null;
		public TextureViewModel TextureViewModel
		{
			get
			{
				if (null == _textureViewModel)
					_textureViewModel = new TextureViewModel();

				return _textureViewModel;
			}
		}

        PerfViewModel _perfViewModel = null;
        public PerfViewModel PerfViewModel
        {
            get
            {
                if (null == _perfViewModel)
                    _perfViewModel = new PerfViewModel();

                return _perfViewModel;
            }
        }

		ScriptViewModel _scriptViewModel = null;
		public ScriptViewModel ScriptViewModel
		{
			get
			{
				if (null == _scriptViewModel)
					_scriptViewModel = new ScriptViewModel();

				return _scriptViewModel;
			}
		}

		ViewPortViewModel _viewPortViewModel = null;
		public ViewPortViewModel ViewPortViewModel
		{
			get
			{
				if (null == _viewPortViewModel)
					_viewPortViewModel = new ViewPortViewModel();

				return _viewPortViewModel;
			}
		}

		ShaderViewModel _shaderViewModel = null;
		public ShaderViewModel ShaderViewModel
		{
			get
			{
				if (null == _shaderViewModel)
					_shaderViewModel = new ShaderViewModel();

				return _shaderViewModel;
			}
		}

        #region data
        private Model.Console _console;
        private Model.Model model_;
        private Model.Perf perf_;
        #endregion

        #region relay
        private ICommand menuNewCommand;
        private ICommand menuOpenCommand;
        private ICommand menuSaveCommand;
        private ICommand menuExitCommand;
        private ICommand menuAddCommand;
        private ICommand treeviewSelectedItemChangedCommand;
        private ICommand menuAddViewportCommand;
        private ICommand menuRemoveViewportCommand;
        private ICommand menuWindowCommand;
		private ICommand keyDeleteCommand;
        private ICommand objectActionCommand;
        private ICommand menuLoadVMFCommand;

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

		public ICommand KeyDeleteCommand
		{
			get { return this.keyDeleteCommand; }
			set { this.keyDeleteCommand = value; }
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

        public ICommand MenuWindowCommand
        {
            get { return this.menuWindowCommand; }
            set { this.menuWindowCommand = value; }
        }

        public ICommand ObjectActionCommand
        {
            get { return this.objectActionCommand; }
            set { this.objectActionCommand = value; }
        }

        public ICommand MenuLoadVMFCommand
        {
            get { return this.menuLoadVMFCommand; }
            set { this.menuLoadVMFCommand = value; }
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
                RaisePropertyChanged("Console");
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
				RaisePropertyChanged("Model");
			}
		}

        public Model.Perf Perf
        {
            get
            {
                return this.perf_;
            }
            set
            {
                this.perf_ = value;
                RaisePropertyChanged("Perf");
            }
        }

		public ManagedLib.ManagedRessources Ressources
		{
			get
			{
				return this.ressources;
			}
			set
			{
				this.ressources = value;
			}
		}

        public void OnLoad(WpfHostingWin32Control.ControlHost controlHost)
        {
            this.Model = new Model.Model(controlHost);
            this.Console = new Model.Console();
            this.Perf = new Model.Perf();
			this.Ressources = new ManagedLib.ManagedRessources();
            this.TreeViewModel.OnLoad(controlHost, this.SelectedEntityChanged);
            this.PropertyViewModel.OnLoad(controlHost, Ressources);
            this.ConsoleViewModel.OnLoad(Console);
			this.MaterialViewModel.OnLoad(this.Ressources);
			this.TextureViewModel.OnLoad(this.Ressources);
            this.PerfViewModel.OnLoad(Perf);
			this.ScriptViewModel.OnLoad(this.Ressources);
			this.ViewPortViewModel.OnLoad(controlHost);
			this.ShaderViewModel.OnLoad();
			this.control = controlHost;
        }

        private void BuildRelay()
        {
            this.MenuNewCommand = new RelayCommand(this.MenuNewExecute);
            this.MenuOpenCommand = new RelayCommand(this.MenuOpenExecute);
            this.MenuSaveCommand = new RelayCommand(this.MenuSaveExecute);
            this.MenuExitCommand = new RelayCommand(param => this.MenuExitExecute());
            this.MenuAddCommand = new RelayCommand(this.MenuAddExecute);
            this.menuAddViewportCommand = new RelayCommand(this.MenuAddViewportExecute);
            this.menuRemoveViewportCommand = new RelayCommand(this.MenuRemoveViewportExecute);
            this.menuWindowCommand = new RelayCommand(this.MenuWindowExecute);
			this.keyDeleteCommand = new RelayCommand(this.KeyDeleteExecute);
            this.objectActionCommand = new RelayCommand(this.ObjectActionExecute);
            this.menuLoadVMFCommand = new RelayCommand(this.MenuLoadVMDExecute);
        }

        private void MenuLoadVMDExecute(object obj)
        {
            string name = null;

            OpenFileDialog dial = new OpenFileDialog();

            dial.Filter = "BSP files (*.BSP)|*.bsp";
            dial.Title = "Select a BSP` File";

            if (System.Windows.Forms.DialogResult.OK == dial.ShowDialog())
            {
                name = dial.FileName;
            }
            else
                return;

            this.control.LoadVmf(name);

            this.TreeViewModel.Entities = this.control.getEntities();
            this.TreeViewModel.SetCallbackEntities(this.control.getEntities());
        }

        #region event handler
        public void MenuNewExecute(object obj)
		{
			control.resetScene();
			this.TreeViewModel.Entities.Clear();
        }

        public void MenuOpenExecute(object obj)
        {
			control.resetScene();
			FileStream ReadFileStream = new FileStream("scene.xml", FileMode.Open, FileAccess.Read, FileShare.Read);
			if (ReadFileStream != null && SerializerObj != null)
			{
				ObservableCollection<SerializableEntity> LoadedObj = SerializerObj.Deserialize(ReadFileStream) as
																	 ObservableCollection<SerializableEntity>;
                LoadSerializableEntity(LoadedObj, IntPtr.Zero);
				ReadFileStream.Close();
				this.TreeViewModel.Entities = control.getEntities();
				if (!LoadSetParameters(LoadedObj, this.TreeViewModel.Entities))
					return;
			}
        }

		private bool LoadSerializableEntity(ObservableCollection<SerializableEntity> entities,
											IntPtr parent, SerializableEntity seriParent = null)
		{
			uint count = 0;
			foreach (SerializableEntity it in entities)
			{
				IntPtr me = IntPtr.Zero;
				if (parent == IntPtr.Zero)
				{
                    if (it.Type == 0 && !File.Exists(this.resourcePath + "\\" + it.Path))
                    {
                        MessageBox.Show("Cannot locate " + it.Path + " in resources directory.");
                        continue;
                    }
                    else
                    {
                        if ((me = control.addEntity(it.Type, null, it.Path)) == IntPtr.Zero)
                            return false;
                    }
				}
				else
				{
                    //if (it.Type == 0)
                    //{
                    //    if (seriParent.Type != 0)
                    //    {
                    //        if ((me = control.addEntityToChildModel(it.Type, parent, count, it.Path)) == IntPtr.Zero)
                    //            return false;
                    //    }
                    //    else if (it.Path != "" &&
                    //             (me = control.addEntity(it.Type, parent, it.Path)) == IntPtr.Zero)
                    //        return false;
                    //}
                    if (it.Type == 0 && !File.Exists(this.resourcePath + "\\" + it.Path))
                    {
                        MessageBox.Show("Cannot locate " + it.Path + " in resources directory.");
                        continue;
                    }
					if ((me = control.addEntity(it.Type, parent)) == IntPtr.Zero)
						return false;
				}
				if (LoadSerializableEntity(it.Childs, me, it) == false)
					return false;
				++count;
			}
			return true;
		}

		private bool LoadSetParameters(ObservableCollection<SerializableEntity> seriEntities,
									   ObservableCollection<ManagedLib.ManagedEntity> entities)
		{
			if (seriEntities == null || entities == null)
				return false;
			int len = entities.Count;
			for (int i = 0; i < len; ++i)
			{
				seriEntities[i].FillEntity(entities[i]);
				if (!LoadSetParameters(seriEntities[i].Childs, entities[i].Childs))
					return false;
			}
			return true;
		}

        public void MenuSaveExecute(object obj)
        {
			ObservableCollection<ManagedLib.ManagedEntity> entities = this.TreeViewModel.Entities;
            if (entities == null)
                return;
            ObservableCollection<SerializableEntity> seriEntities = GetSerializableEntities(entities);
            if (SerializerObj != null)
            {
                TextWriter WriteFileStream = new StreamWriter("scene.xml");
                if (WriteFileStream != null)
                {
                    SerializerObj.Serialize(WriteFileStream, seriEntities);
                    WriteFileStream.Close();
                }
            }
        }

		private ObservableCollection<SerializableEntity>
        GetSerializableEntities(ObservableCollection<ManagedLib.ManagedEntity> entities)
        {
            ObservableCollection<SerializableEntity> seriEntities = new ObservableCollection<SerializableEntity>();
            foreach (ManagedLib.ManagedEntity entity in entities)
            {
                SerializableEntity seriEntity = new SerializableEntity(entity);
				seriEntity.Childs = GetSerializableEntities(entity.Childs);
                /*if (!File.Exists(seriEntity.Path))
                {
                    MessageBox.Show("Unable to locate file " + this.resourcePath + "\\" + seriEntity.Path + ". Entity will not be load");
                }
                else
                {*/
                    seriEntities.Add(seriEntity);
                //}
            }
            return seriEntities;
        }

		public Action MenuExitAction { get; set; }
        public void MenuExitExecute()
        {
			MenuExitAction();
        }

        public void MenuAddExecute(object obj)
        {
            this.AddModel(Convert.ToInt32(obj));
        }

        //public void TreeViewSelectedItemChangedExecute(object obj)
        //{
        //    this.model_.SelectedEntity = obj as ManagedLib.ManagedEntity;
        //}

        //public void AnimationChangedExecute(object obj)
        //{
        //    this.AnimationChanged(Convert.ToInt32(obj));
        //}

        //public void ViewportSelectionExecute(object obj)
        //{
        //    this.ViewportSelection(Convert.ToInt32(obj));
        //}

        public void MenuAddViewportExecute(object obj)
        {
            this.ViewportAdd();
        }

        public void MenuRemoveViewportExecute(object obj)
        {
            this.ViewportRemove();
        }

        public void MenuWindowExecute(object obj)
        {
            string code = Convert.ToString(obj);

            if (null == code)
                return;

            this.AddTool(code);
        }

		public void KeyDeleteExecute(object obj)
		{
			if (TreeViewModel.SelectedEntity == null)
				return;
			Model.GetManaged().removeEntity(TreeViewModel.SelectedEntity);
			TreeViewModel.Entities = Model.GetManaged().getEntities();
		}

        public void ObjectActionExecute(object obj)
        {
            string option = Convert.ToString(obj);

            if (null == option)
                return;

            this.TreeViewModel.OptionChanged(option);
        }
        #endregion

        public void SelectedEntityChanged(ManagedLib.ManagedEntity entity)
        {
			this.AnimationViewModel.SelectedEntity = null;
			this.PropertyViewModel.SelectedEntity = null;
			this.MaterialViewModel.SelectedEntity = null;
            this.AnimationViewModel.SelectedEntity = entity;
            this.PropertyViewModel.SelectedEntity = entity;
			this.MaterialViewModel.SelectedEntity = entity;

            if (this.Autoload)
            {
                if (null == entity)
                {
                    this.AnimationViewModel.IsVisible = false;
                    this.PropertyViewModel.IsVisible = false;
					this.MaterialViewModel.IsVisible = false;
                }
                else
                {
                    if (null != entity && entity.AnimationNames.Count > 0)
                        this.AnimationViewModel.IsVisible = true;
                    else
                        this.AnimationViewModel.IsVisible = false;
					this.MaterialViewModel.IsVisible = true;
                    this.PropertyViewModel.IsVisible = true;
                }
            }
        }

        private void AddTool(string code)
        {
            if ("Entity" == code)
            {
                this.TreeViewModel.IsVisible = !this.TreeViewModel.IsVisible;
            }
            else if ("Property" == code)
            {
                this.PropertyViewModel.IsVisible = !this.PropertyViewModel.IsVisible;
            }
            else if ("DebugConsole" == code)
            {
                this.ConsoleViewModel.IsVisible = !this.ConsoleViewModel.IsVisible;
            }
            else if ("Animation" == code)
            {
                this.AnimationViewModel.IsVisible = !this.AnimationViewModel.IsVisible;
            }
			else if ("Materials" == code)
			{
				this.MaterialViewModel.IsVisible = !this.MaterialViewModel.IsVisible;
			}
			else if ("Textures" == code)
			{
				this.TextureViewModel.IsVisible = !this.TextureViewModel.IsVisible;
			}
        }

        private void AddModel(int type)
        {
            string name = null;

            if (0 == type)
            {
                OpenFileDialog dial = new OpenFileDialog();

                dial.Filter = "FBX files (*.FBX)|*.fbx";
                dial.Title = "Select a FBX File";

                if (System.Windows.Forms.DialogResult.OK == dial.ShowDialog())
                {
                    name = dial.FileName;

                    //name = this.CopyToResource(dial.FileName, dial.SafeFileName);
                }
                else
                    return;
            }
            this.TreeViewModel.OnAddModel(type, name);
            //bool ret = await this.TreeViewModel.AddModel(type, name);
            if (this.Autoload)
                this.TreeViewModel.IsVisible = true;
        }

        private string CopyToResource(string path, string filename)
        {
            string dest = this.resourcePath + "\\" + filename;

            try
            {
                File.Copy(path, dest, true);
            }
            catch (UnauthorizedAccessException e)
            {
            }

            return dest;
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
        }

        private bool _autoload;
        public bool Autoload
        {
            get { return _autoload; }
            set
            {
                if (value != _autoload)
                    _autoload = value;

                RaisePropertyChanged("Autoload");
            }
        }
    }
}
