using System;

namespace HabitTracker.Models
{
    public class Habit
    {
        public string Name { get; set; }
        public int CompletedDays { get; set; }
        public DateTime LastCompletedDate { get; set; }

        public Habit(string name)
        {
            Name = name;
            CompletedDays = 0;
        }

        public void MarkCompleted()
        {
            if (LastCompletedDate.Date != DateTime.Today)
            {
                CompletedDays++;
                LastCompletedDate = DateTime.Today;
            }
        }
    }
}