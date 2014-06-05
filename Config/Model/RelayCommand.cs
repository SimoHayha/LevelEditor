using System;
using System.Windows.Input;

namespace Config.Model
{
    public class RelayCommand : ICommand
    {
        readonly Action<Object> execute_;
        readonly Predicate<Object> canExecute_;

        public RelayCommand(Action<Object> execute)
        {
            this.execute_    = execute;
            this.canExecute_ = null;
        }

        public RelayCommand(Action<Object> execute, Predicate<object> canExecute)
        {
            if (null == execute)
                throw new ArgumentNullException("execute");

            this.execute_    = execute;
            this.canExecute_ = canExecute;
        }

        public bool CanExecute(object parameter)
        {
            return null == this.canExecute_ ? true : this.canExecute_(parameter);
        }

        public event EventHandler CanExecuteChanged
        {
            add
            {
                CommandManager.RequerySuggested += value;
            }
            remove
            {
                CommandManager.RequerySuggested -= value;
            }
        }

        public void Execute(object parameter)
        {
            this.execute_(parameter);
        }
    }
}
