// 最大数を持ったキュー。古いデータは端から「落ちる」。
class ConveyorQueue {
    queue<int> q;
    int max_items;
    int total_sum; // qに含まれるすべての項目の合計

    public:
        ConveyorQueue(int max_items) : max_items(max_items), total_sum(0) {
        }

        // キューの値を'num_shifted'の分だけシフトする。
        // 新しい項目は0で初期化する。
        // 最古の項目はmax_items以下なら削除する。
        void Shift(int num_shifted) {
            // 項目がシフトされすぎた場合に、キューをクリアする。
            if (num_shifted >= max_items) {
                q = queue<int>(); // キューをクリア
                total_sum = 0;
                return;
            }

            // 必要な分だけ0をプッシュする。
            while (num_shifted > 0) {
                q.push(0);
                num_shifted--;
            }

            // 超過した項目はすべて落とす。
            while (q.size() > max_items) {
                total_sum -= q.front();
                q.pop();
            }
        }

        // キューの最後の値を増加する。
        void AddToBack(int count) {
            if (q.empty()) Shift(1); // qが少なくとも1つの項目を持つようにする。
            q.back() += count;

            total_sum += count;
        }

        // 現在のキューに含まれる項目の合計数を返す。
        int TotalSum() {
            return total_sum;
        }
};

// 時間バケツN個のカウントを保持するクラス。
class TrailingBucketCounter {
    ConveyorQueue buckets;
    const int secs_per_bucket;
    time_t last_update_time; // Update() が最後に呼び出された時刻

    // 通過した時間バケツの数を計算して Shift() する。
    void Update(time_t now) {
        int current_bucket = now / secs_per_bucket;
        int last_update_bucket = last_update_time / secs_per_bucket;

        buckets.Shift(current_bucket - last_update_bucket);
        last_update_time = now;
    }

    public:
        // 例: TrailingBucketCounter(30, 60)は、直近30分のバケツを追跡する。
        TrailingBucketCounter(int num_buckets, int secs_per_bucket) :
            buckets(num_buckets),
            secs_per_bucket(secs_per_bucket) {
        }

        void Add(int count, time_t now) {
            Update(now);
            buckets.AddToBack(count);
        }

        // 最新のnum_bucketsの時間に含まれる合計カウントを返す
        int TrailingCount(time_t now) {
            Update(now);
            return buckets.TotalSum();
        }
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
