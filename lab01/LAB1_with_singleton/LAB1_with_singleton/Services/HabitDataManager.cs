using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using HabitTracker.Models;

namespace HabitTracker.Services
{
    public sealed class HabitDataManager
    {
        private static HabitDataManager _instance;
        private static readonly object _lock = new object();
        private const string FilePath = "habits.json";

        public List<Habit> Habits { get; private set; }

        private HabitDataManager()
        {
            Load();
        }

        public static HabitDataManager Instance
        {
            get
            {
                lock (_lock)
                {
                    if (_instance == null)
                        _instance = new HabitDataManager();

                    return _instance;
                }
            }
        }

        public void AddHabit(Habit habit)
        {
            Habits.Add(habit);
            Save();
        }

        public void RemoveHabit(Habit habit)
        {
            Habits.Remove(habit);
            Save();
        }

        public void Save()
        {
            var json = JsonSerializer.Serialize(Habits, new JsonSerializerOptions
            {
                WriteIndented = true
            });

            File.WriteAllText(FilePath, json);
        }

        private void Load()
        {
            if (File.Exists(FilePath))
            {
                var json = File.ReadAllText(FilePath);
                Habits = JsonSerializer.Deserialize<List<Habit>>(json);
            }
            else
            {
                Habits = new List<Habit>();
            }
        }
    }
}