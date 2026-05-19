# Shortest Path in Maze - Açıklama ve Kullanım

Bu proje, farklı arama algoritmalarını kullanarak rastgele üretilmiş bir labirentte başlangıç noktasından hedefe en kısa yolu bulan C kodları içerir.

Dosyalar:
- `data_types.h`: Problem spesifik veri tipleri (`State`, `Node`, `Queue`) ve, labirent boyutlarını tutan `extern` değişkenler.
- `GRAPH_SEARCH.c`: Programın `main()` fonksiyonunu içerir; kullanıcıdan algoritma ve parametreleri alır, aramayı başlatır ve çözümü görselleştirir.
- `GRAPH_SEARCH.h`: Arama algoritmaları için gereken fonksiyon bildirimleri ve sabitler.
- `Standart_Search.c`: Genel arama iskeleti (FIFO, LIFO, öncelikli kuyruklar, düğüm oluşturma, frontier işlemleri, vb.).
- `HashTable.c` ve `hashTable.h`: Durumların keşfedilip daha önce ziyaret edilip edilmediğini kontrol etmek için basit bir hash tablosu implementasyonu.
- `SpecificToProblem.c`: Labirent oluşturma (recursive backtracker), `Create_State`, `Result`, `Compute_Heuristic_Function`, `Visualize_Solution` gibi problem-spesifik fonksiyonlar.

Derleme ve çalıştırma (Windows/GCC):

```bash
gcc -o maze_solver GRAPH_SEARCH.c Standart_Search.c HashTable.c SpecificToProblem.c -lm
./maze_solver
```

GitHub'a push etme (komut satırı): uzak depo URL'sini kendi GitHub deponuz ile değiştirin.

```bash
git init
git add .
git commit -m "Add maze solver project"
git branch -M main
git remote add origin https://github.com/USERNAME/REPO.git
git push -u origin main
```

Notlar:
- `SpecificToProblem.c` labirenti rastgele üretir ve kullanıcıdan başlangıç/hedef konumunu ister.
- `Standart_Search.c` dosyası çeşitli arama stratejilerini uygular; A*, Greedy, Uniform Cost ve BFS/DFS seçenekleri mevcuttur.
- Push işlemi için GitHub kimlik doğrulaması (PAT veya SSH anahtarı) gereklidir; bu betik kimlik bilgilerini içermez.

İsterseniz ben local olarak commit atmayı ve (eğer erişim sağlarsanız) push etmeyi deneyebilirim; yoksa yukarıdaki adımları kendiniz çalıştırabilirsiniz.
