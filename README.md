# Maze Shortest Path — Graph Search Algorithms

Bu proje, rastgele üretilen bir labirentte başlangıç noktasından hedef noktasına giden en kısa (veya optimal) yolu çeşitli graf arama algoritmaları ile bulmaktadır. C diliyle yazılmıştır ve modüler bir yapıya sahiptir.

---

## Dosya Yapısı

```
.
├── data_types.h            # Temel veri tipleri (State, Node, Queue, vb.)
├── hashTable.h             # Hash tablosu arayüzü
├── HashTable.c             # Hash tablosu implementasyonu
├── GRAPH_SEARCH.h          # Arama algoritmaları arayüzü
├── GRAPH_SEARCH.c          # main() fonksiyonu
├── Standart_Search.c       # Genel arama algoritmaları (değiştirilmez)
└── SpecificToProblem.c     # Probleme özgü fonksiyonlar (labirent mantığı)
```

---

## Veri Yapıları (`data_types.h`)

### `State`
Labirentteki bir konumu temsil eder.

| Alan  | Tip   | Açıklama                          |
|-------|-------|-----------------------------------|
| `row` | int   | Satır indeksi (0 ... MAZE_ROWS-1) |
| `col` | int   | Sütun indeksi (0 ... MAZE_COLS-1) |
| `h_n` | float | Sezgisel fonksiyon değeri (A* için) |

### `Node`
Arama ağacındaki bir düğümü temsil eder.

| Alan              | Açıklama                                      |
|-------------------|-----------------------------------------------|
| `state`           | Düğümün labirentteki konumu                   |
| `path_cost`       | Kökten bu düğüme kadar olan toplam maliyet    |
| `action`          | Bu düğüme ulaşmak için uygulanan hareket      |
| `parent`          | Ebeveyn düğüm işaretçisi                      |
| `Number_of_Child` | Çocuk düğüm sayısı (DFS bellek yönetimi için) |

### `ACTIONS` (enum)
Labirente uygulanabilecek 4 hareket:
- `Move_Up` (0) — Yukarı (row - 1)
- `Move_Down` (1) — Aşağı (row + 1)
- `Move_Left` (2) — Sola (col - 1)
- `Move_Right` (3) — Sağa (col + 1)

---

## Desteklenen Arama Algoritmaları

| No | Algoritma                   | Yöntem                         | Optimal? |
|----|-----------------------------|--------------------------------|----------|
| 1  | Breadth-First Search (BFS)  | `First_GoalTest_Search_TREE`   | Evet (birim maliyetle) |
| 2  | Uniform Cost Search (UCS)   | `First_InsertFrontier_Search_TREE` | Evet |
| 3  | Depth-First Search (DFS)    | `DepthType_Search_TREE`        | Hayır    |
| 4  | Depth-Limited Search (DLS)  | `DepthType_Search_TREE`        | Hayır    |
| 5  | Iterative Deepening (IDS)   | `DepthType_Search_TREE` (döngü) | Evet   |
| 6  | Greedy Search               | `First_GoalTest_Search_TREE`   | Hayır    |
| 7  | A* Search                   | `First_InsertFrontier_Search_TREE` | Evet |
| 8  | Generalized A* Search       | `First_InsertFrontier_Search_TREE` | α'ya bağlı |

### Generalized A* Değerlendirme Fonksiyonu

```
f(n) = (1 - α) * g(n)  +  α * h(n)
```

- `α = 0.0` → Uniform Cost Search (yalnızca g)
- `α = 0.5` → Dengeli A* 
- `α = 1.0` → Greedy Search (yalnızca h)

### Sezgisel Fonksiyon (Heuristic)

Manhattan mesafesi kullanılmaktadır:

```c
h(n) = |row_n - row_goal| + |col_n - col_goal|
```

Bu sezgisel, 4-yönlü harekette kabul edilebilir (admissible) ve tutarlıdır (consistent), bu nedenle A* optimal sonuç verir.

---

## Labirent Yapısı

### Boyutlar

Program başlangıçta labirent boyutunu kullanıcıdan alır:

| Seçenek | Oyun Alanı  | Dahili Grid |
|---------|-------------|-------------|
| Small   | 20 × 20     | 21 × 21     |
| Large   | 50 × 50     | 51 × 51     |

### Üretim Algoritması

Labirent **Recursive Backtracker** (Özyinelemeli Geri İzleme) algoritmasıyla rastgele üretilir. Grid her zaman tek sayı × tek sayı boyutundadır; odalar tek indeksli konumlarda yer alır. Varsayılan olarak sabit tohum (`srand(15)`) kullanılır, böylece her çalıştırmada aynı labirent elde edilir. Farklı labirentler için `SpecificToProblem.c` içinde `srand(time(NULL))` satırını aktif edebilirsiniz.

### Semboller

| Sembol | Anlamı     |
|--------|------------|
| `##`   | Duvar      |
| `  `   | Açık hücre |
| `\|\|` | Çözüm yolu |
| `SS`   | Başlangıç  |
| `GG`   | Hedef      |

---

## Hash Tablosu (`HashTable.c`)

Keşfedilen durumları O(1) ortalama karmaşıklıkla takip etmek için açık adresleme (linear probing) kullanan bir hash tablosudur.

| Parametre                    | Değer | Açıklama                              |
|------------------------------|-------|---------------------------------------|
| `HASH_TABLE_BASED_SIZE`      | 29    | Başlangıç kapasitesi (asal sayı)      |
| `HASH_TABLE_INCREASING_RATE` | 70    | Yeniden boyutlandırma eşiği (%70 dolu) |
| `MAX_KEY_SIZE`               | 5     | Anahtar uzunluğu (50×50 grid için yeterli) |

Anahtar üretimi: `key = row * MAZE_COLS + col` (doğrusal indeks → string)

---

## Derleme ve Çalıştırma

```bash
gcc -o maze GRAPH_SEARCH.c Standart_Search.c SpecificToProblem.c HashTable.c -lm
./maze
```

Çalıştırıldığında sırasıyla şunlar sorulur:

1. Algoritma seçimi (1–8)
2. DLS için maksimum derinlik (sadece algoritma 4 seçilirse)
3. Generalized A* için α değeri (sadece algoritma 8 seçilirse)
4. Labirent boyutu (Small / Large)
5. Başlangıç konumu (satır, sütun)
6. Hedef konumu (satır, sütun)

---

## Çıktı

Program çözüm bulduğunda şunları raporlar:

- Aranan düğüm sayısı
- Üretilen düğüm sayısı
- Bellekteki düğüm sayısı
- Çözüm yolunun adım adım listesi
- Toplam yol maliyeti
- Labirent üzerinde görsel çözüm haritası

---

## Modüler Genişletilebilirlik

Yeni bir arama problemi eklemek için yalnızca şu fonksiyonları güncelleyin (`SpecificToProblem.c`):

| Fonksiyon                    | Açıklama                                           |
|------------------------------|----------------------------------------------------|
| `Create_State()`             | Kullanıcıdan durum oku, belleğe al                 |
| `Result()`                   | Bir eylem sonucu oluşan yeni durumu hesapla        |
| `Goal_Test()`                | Hedef duruma ulaşılıp ulaşılmadığını kontrol et    |
| `Compute_Heuristic_Function()` | Sezgisel fonksiyon değerini hesapla              |
| `Print_State()` / `Print_Action()` | Durum ve eylem yazdırma                    |

`Standart_Search.c` ve `HashTable.c` dosyaları probleme özgü değildir ve değiştirilmesi gerekmez.
