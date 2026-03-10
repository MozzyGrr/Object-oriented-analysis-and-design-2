using System;
using System.Windows;
using System.Windows.Threading;
using HabitTracker.Models;
using HabitTracker.Services;

namespace HabitTracker
{
    public partial class MainWindow : Window
    {
        private DispatcherTimer reminderTimer;

        public MainWindow()
        {
            InitializeComponent();
            HabitList.ItemsSource = HabitDataManager.Instance.Habits;
            reminderTimer = new DispatcherTimer();
            reminderTimer.Interval = TimeSpan.FromSeconds(30);
            reminderTimer.Tick += ReminderTimer_Tick;
            reminderTimer.Start();
        }

        private void AddHabit_Click(object sender, RoutedEventArgs e)
        {
            string name = Microsoft.VisualBasic.Interaction.InputBox("Название привычки:", "Новая привычка");
            string intervalStr = Microsoft.VisualBasic.Interaction.InputBox("Интервал напоминания (в минутах):", "Напоминание");
            string targetStr = Microsoft.VisualBasic.Interaction.InputBox("Сколько раз в день выполнять?", "Цель");

            if (int.TryParse(intervalStr, out int minutes) &&
                int.TryParse(targetStr, out int target))
            {
                Habit habit = new Habit
                {
                    Name = name,
                    ReminderIntervalMinutes = minutes,
                    DailyTarget = target,
                    IsReminderEnabled = true,
                    NextReminderTime = DateTime.Now.AddMinutes(minutes)
                };

                HabitDataManager.Instance.AddHabit(habit);
                HabitList.Items.Refresh();
            }
        }

        private void CompleteHabit_Click(object sender, RoutedEventArgs e)
        {
            if (HabitList.SelectedItem is Habit habit)
            {
                habit.MarkCompleted();
                HabitDataManager.Instance.Save();
                HabitList.Items.Refresh();
            }
        }

        private void DeleteHabit_Click(object sender, RoutedEventArgs e)
        {
            if (HabitList.SelectedItem is Habit habit)
            {
                HabitDataManager.Instance.RemoveHabit(habit);
                HabitList.Items.Refresh();
            }
        }

        private void ReminderTimer_Tick(object sender, EventArgs e)
        {
            foreach (var habit in HabitDataManager.Instance.Habits)
            {
                habit.ResetIfNewDay();

                if (habit.IsReminderEnabled && !habit.IsCompletedForToday() && DateTime.Now >= habit.NextReminderTime)
                {
                    MessageBox.Show($"Пора выполнить: {habit.Name}");

                    habit.NextReminderTime = DateTime.Now.AddMinutes(habit.ReminderIntervalMinutes);
                    HabitDataManager.Instance.Save();
                }
            }
        }
    }
}