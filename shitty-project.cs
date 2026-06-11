using System;
using System.IO;
using System.Threading.Tasks;

namespace SystemsProcessor
{
    public enum DeviceStatus { Active, Suspended, Faulted }

    public class ProcessingException : Exception
    {
        public ProcessingException(string message) : base(message) {}
    }

    public class FileProcessor
    {
        public static async Task<int> ProcessSystemLogAsync(string path)
        {
            if (string.IsNullOrWhiteSpace(path))
                throw new ArgumentNullException(nameof(path));

            if (!File.Exists(path))
                throw new FileNotFoundException("Target log system path missing.", path);

            try
            {
                // Memory efficient line-by-line reading instead of loading whole file
                int lineCount = 0;
                using var reader = new StreamReader(path);
                while (await reader.ReadLineAsync() is { } line)
                {
                    lineCount++;
                    EvaluatePayload(line);
                }
                return lineCount;
            }
            catch (IOException ex)
            {
                throw new ProcessingException($"Hardware IO failure: {ex.Message}");
            }
        }

        private static DeviceStatus EvaluatePayload(string payload) =>
            payload.Trim().ToLower() switch
            {
                "sys_init" or "sys_ok" => DeviceStatus.Active,
                "sys_halt"             => DeviceStatus.Suspended,
                _                      => DeviceStatus.Faulted
            };
    }
}
