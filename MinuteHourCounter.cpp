// 時間バケツN個のカウントを保持するクラス。
class TrailingBucketCounter {
    public:
        // 例: TrailingBucketCounter(30, 60)は、直近30分のバケツを追跡する。
        TrailingBucketCounter(int num_buckets, int secs_per_bucket);
      
        void Add(int count, time_t now);

        // 最新のnum_bucketsの時間に含まれる合計カウントを返す
        int TrailingCount(time_t now);
};

// 直近1分間および直近1時間の累積カウントを記録する。
// 例えば、帯域幅の使用状況を確認するのに使える。
class MinuteHourCounter
{
    TrailingBucketCounter minute_counts;
    TrailingBucketCounter hour_counts;

    public:
        MinuteHourCounter() :
            minute_counts(/* num_buckets = */ 60, /* secs_per_bucket =  */ 1),
            hour_counts(/* num_buckets = */ 60, /* secs_per_bucket =  */ 60) {
        }

        // 新しいデータ点を追加する (count >= 0)。
        // それから1分間は、MinuteCount()の返す値が+count分だけ増える。
        // それから1時間は、HourCount()の返す値が+count分だけ増える。
        void Add(int count) {
            time_t now = time();
            minute_counts.Add(count, now);
            hour_counts.Add(count,now);

        }

        // 直近60秒間の累積カウントを返す。
        int MinuteCount() {
            time_t now = time();
            return minute_counts.TrailingCount(now);
        }

        // 直近3600秒間の累積カウントを返す。
        int HourCount() {
            time_t now = time();
            return hour_counts.TrailingCount(now);
        }
};
