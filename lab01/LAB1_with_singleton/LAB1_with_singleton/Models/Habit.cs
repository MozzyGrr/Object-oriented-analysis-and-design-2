using System;

namespace HabitTracker.Models
{
    public class Habit
    {
        public string Name { get; set; }

        public int ReminderIntervalMinutes { get; set; }

        public bool IsReminderEnabled { get; set; }

        public int DailyTarget { get; set; }

        public int TodayCompletions { get; set; }

        public int TotalCompletions { get; set; }

        public DateTime LastCompletionDate { get; set; }

        public DateTime NextReminderTime { get; set; }

        public void MarkCompleted()
        {
            if (LastCompletionDate.Date != DateTime.Today)
            {
                TodayCompletions = 0;
            }

            TodayCompletions++;
            TotalCompletions++;
            LastCompletionDate = DateTime.Today;
        }

        public void ResetIfNewDay()
        {
            if (LastCompletionDate.Date != DateTime.Today)
            {
                TodayCompletions = 0;
            }
        }

        public bool IsCompletedForToday()
        {
            return TodayCompletions >= DailyTarget;
        }
    }
}