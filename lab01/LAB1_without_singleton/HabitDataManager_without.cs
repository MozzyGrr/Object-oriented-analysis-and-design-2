using System.Collections.ObjectModel;
using HabitTracker.Models;

namespace HabitTracker.Services
{
    public class HabitDataManager
    {
        public ObservableCollection<Habit> Habits { get; }

        public HabitDataManager()
        {
            Habits = new ObservableCollection<Habit>();
        }

        public void AddHabit(Habit habit)
        {
            Habits.Add(habit);
        }

        public void RemoveHabit(Habit habit)
        {
            Habits.Remove(habit);
        }

        public void Save()
        {

        }
    }
}