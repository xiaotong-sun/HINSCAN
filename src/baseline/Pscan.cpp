/**
 * @file pScan.cpp
 * @author Xiaotong Sun
 * @date 2023-11-08
 *
 */

#include "Pscan.h"

Pscan::Pscan(const unordered_map<int, set<int>>& homoGraph, const map<int, vector<int>>& hinGraph, const vector<int>& vertexType, const vector<int>& edgeType, const unordered_map<int, int>& edgeReverseMap, const MetaPath& metaPath, int mode) : homoGraph(homoGraph), hinGraph(hinGraph), vertexType(vertexType), edgeType(edgeType), edgeReverseMap(edgeReverseMap), metaPath(metaPath), mode(mode) {
    n = m = 0;
    eps_a2 = eps_b2 = miu = 0;
    index2id = nullptr;

    pstart = nullptr;
    edges = nullptr;
    reverse = nullptr;
    min_cn = nullptr;

    cid = nullptr;

    degree = nullptr;
    effective_degree = nullptr;
    similar_degree = nullptr;

    pa = nullptr;
    rank = nullptr;
}

Pscan::~Pscan() {
    if (index2id != nullptr) {
        delete[] index2id;
        index2id = nullptr;
    }
    if (pstart != nullptr) {
        delete[] pstart;
        pstart = nullptr;
    }
    if (edges != nullptr) {
        delete[] edges;
        edges = nullptr;
    }
    if (reverse != nullptr) {
        delete[] reverse;
        reverse = nullptr;
    }
    if (min_cn != nullptr) {
        delete[] min_cn;
        min_cn = nullptr;
    }
    if (cid != nullptr) {
        delete[] cid;
        cid = nullptr;
    }
    if (degree != nullptr) {
        delete[] degree;
        degree = nullptr;
    }
    if (effective_degree != nullptr) {
        delete[] effective_degree;
        effective_degree = nullptr;
    }
    if (similar_degree != nullptr) {
        delete[] similar_degree;
        similar_degree = nullptr;
    }
    if (pa != nullptr) {
        delete[] pa;
        pa = nullptr;
    }
    if (rank != nullptr) {
        delete[] rank;
        rank = nullptr;
    }
}

void Pscan::get_graph() {
    n = homoGraph.size(); // number of vertices
    if (degree == nullptr) {
        degree = new int[n];
    }
    if (index2id == nullptr) {
        index2id = new ui[n];
    }

    int i = 0;
    for (auto& item : homoGraph) {
        degree[i] = item.second.size() - 1;
        index2id[i] = item.first;
        id2index[item.first] = i;
        m += degree[i];
        i++;
    }

    if (pstart == nullptr) {
        pstart = new ui[n + 1];
    }
    if (edges == nullptr) {
        edges = new int[m];
    }
    if (reverse == nullptr) {
        reverse = new ui[m];
    }
    if (min_cn == nullptr) {
        min_cn = new int[m];
    }
    memset(min_cn, 0, sizeof(int) * m);

    int* buf = new int[n];

    pstart[0] = 0;
    i = 0;
    for (auto& item : homoGraph) {
        // printf("index = %d \t degree = %d\n", i, degree[i]);
        set<int> neighborTemp; // this set is used to make neighbor node in increasing order.

        for (auto& elem : item.second) {
            if (item.first == elem) {
                continue;
            }
            neighborTemp.insert(id2index[elem]);
        }

        ui k = 0;
        for (int elem : neighborTemp) {
            buf[k] = elem;
            k++;
        }

        for (ui j = 0; j < degree[i]; j++) {
            edges[pstart[i] + j] = buf[j];
        }

        pstart[i + 1] = pstart[i] + degree[i];

        ++degree[i]; // add the node itself?
        ++i;
    }

    delete[] buf;

    for (ui i = 0; i < n; i++) {
        for (ui j = pstart[i]; j < pstart[i + 1]; j++) {
            if (edges[j] == i) {
                cout << "Self Loop" << endl;
            }
            if (j > pstart[i] && edges[j] <= edges[j - 1]) {
                cout << "Edges not sorted in increasing order!" << endl;
                cout << "The program may not run properly!" << endl;
            }
        }
    }
}

ui Pscan::binary_search(const int* array, ui b, ui e, int val) {
    --e;
    if (array[e] < val) return e + 1;
    while (b < e) {
        ui mid = b + (e - b) / 2;
        if (array[mid] >= val) e = mid;
        else b = mid + 1;
    }
    return e;
}

void Pscan::cluster_noncore_vertices(int eps_a2, int eps_b2, int mu) {
    if (cid == nullptr) cid = new int[n];
    for (ui i = 0;i < n;i++) cid[i] = n;

    for (ui i = 0;i < n;i++) if (similar_degree[i] >= mu) {
        int x = find_root(i);
        if (i < cid[x]) cid[x] = i;
    }

    noncore_cluster.clear();
    noncore_cluster.reserve(n);
    for (ui i = 0;i < n;i++) if (similar_degree[i] >= mu) {
        for (ui j = pstart[i];j < pstart[i + 1];j++) if (similar_degree[edges[j]] < mu) {
            if (min_cn[j] >= 0) {
                min_cn[j] = similar_check_OP(i, j, eps_a2, eps_b2);
                if (reverse[reverse[j]] != j) printf("WA cluster_noncore\n");
                min_cn[reverse[j]] = min_cn[j];
                if (min_cn[j] == -1) {
                    ++similar_degree[i];
                    ++similar_degree[edges[j]];
                }
            }

            if (min_cn[j] == -1) noncore_cluster.push_back(make_pair(cid[pa[i]], edges[j]));
        }
    }
}

void Pscan::output(const char* eps_s, const char* miu, string dir) {
    string out_name = dir + "/PscanResult-" + string(eps_s) + "-" + string(miu) + "-" + to_string(mode) + ".txt";
    FILE* fout = open_file(out_name.c_str(), "w");

    fprintf(fout, "c/n vertex_id cluster_id\n");

    int mu = atoi(miu);

    // for (ui i = 0; i < n; i++) {
    //     cout << index2id[i] << ": " << similar_degree[i] << "\n";
    // }

    for (ui i = 0;i < n;i++) if (similar_degree[i] >= mu) {
        fprintf(fout, "c %d %d\n", index2id[i], cid[pa[i]]);
    }

    sort(noncore_cluster.begin(), noncore_cluster.end());
    noncore_cluster.erase(unique(noncore_cluster.begin(), noncore_cluster.end()), noncore_cluster.end());
    for (ui i = 0;i < noncore_cluster.size();i++) {
        fprintf(fout, "n %d %d\n", index2id[noncore_cluster[i].second], noncore_cluster[i].first);
    }

    fclose(fout);
    cout << "Finish writing Pscan-Result to " << out_name << endl;
}

void Pscan::pSCAN(const char* eps_s, int _miu) {
    get_eps(eps_s);
    miu = _miu;

    if (similar_degree == nullptr) similar_degree = new int[n];
    for (ui i = 0; i < n; i++) similar_degree[i] = 1;
    // 我认为原代码这里存在一些问题，做如下改动
    // memset(similar_degree, 0, sizeof(int) * n);

    if (effective_degree == nullptr) effective_degree = new int[n];
    // for (ui i = 0;i < n;i++) effective_degree[i] = degree[i] - 1;
    for (ui i = 0;i < n;i++) effective_degree[i] = degree[i];

    if (pa == nullptr) pa = new int[n];
    if (rank == nullptr) rank = new int[n];
    for (ui i = 0;i < n;i++) {
        pa[i] = i;
        rank[i] = 0;
    }

    ui* edge_buf = new ui[n];
    int* cores = new int[n];
    int cores_n = 0;

    prune_and_cross_link(eps_a2, eps_b2, miu, cores, cores_n);
    // printf("\t*** Finished prune and cross link!\n");

    int* bin_head = new int[n + 1];
    int* bin_next = new int[n];
    for (ui i = 0;i < n + 1;i++) bin_head[i] = -1;

    // 下面这段有什么用？bin_head, bin_next是干什么的？
    int max_ed = 0;
    for (ui i = 0;i < n;i++) if (effective_degree[i] >= miu) {
        int ed = effective_degree[i];
        if (ed > max_ed) max_ed = ed;
        bin_next[i] = bin_head[ed];
        bin_head[ed] = i;
    }

    while (true) {
        int u = -1;
        if (cores_n) u = cores[--cores_n];
        else { // 这个else有什么用？
            while (max_ed >= miu && u == -1) {
                for (int x = bin_head[max_ed];x != -1;) {
                    int tmp = bin_next[x];
                    int ed = effective_degree[x];
                    if (ed == max_ed) {
                        u = x;
                        bin_head[max_ed] = bin_next[x];
                        break;
                    } else if (ed >= miu) {
                        bin_next[x] = bin_head[ed];
                        bin_head[ed] = x;
                    }
                    x = tmp;
                }
                if (u == -1) {
                    bin_head[max_ed] = -1;
                    --max_ed;
                }
            }
        }
        if (u == -1) break;

        int edge_buf_n = 0;
        for (ui j = pstart[u];j < pstart[u + 1];j++) {
            if (min_cn[j] == -2) continue;

            if (similar_degree[u] < miu || find_root(u) != find_root(edges[j])) edge_buf[edge_buf_n++] = j;
        }

        int i = 0;
        while (similar_degree[u] < miu && effective_degree[u] >= miu && i < edge_buf_n) {
            ui idx = edge_buf[i];
            if (min_cn[idx] != -1) {
                int v = edges[idx];

                min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2); // similar_chech_OP only return -1 or -2;

                if (min_cn[idx] == -1) ++similar_degree[u];
                else --effective_degree[u];

                if (effective_degree[v] >= 0) {
                    if (min_cn[idx] == -1) {
                        ++similar_degree[v];

                        if (similar_degree[v] == miu) cores[cores_n++] = v;
                    } else --effective_degree[v];
                }
            }

            ++i;
        }

        effective_degree[u] = -1; // u is a core.

        if (similar_degree[u] < miu) continue;

        for (int j = 0;j < edge_buf_n;j++) {
            ui idx = edge_buf[j];
            if (min_cn[idx] == -1 && similar_degree[edges[idx]] >= miu) my_union(u, edges[idx]);
        }

        while (i < edge_buf_n) {
            ui idx = edge_buf[i];
            int v = edges[idx];
            if (min_cn[idx] < 0 || similar_degree[v] < miu || find_root(u) == find_root(v)) {
                ++i;
                continue;
            }

            min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2);

            if (effective_degree[v] >= 0) { // if < 0 , means it is a core?
                if (min_cn[idx] == -1) {
                    ++similar_degree[v];

                    if (similar_degree[v] == miu) cores[cores_n++] = v;
                } else --effective_degree[v];
            }

            if (min_cn[idx] == -1) my_union(u, v);

            ++i;
        }
        //printf(")\n");
    }
    //printf("\t*** Finished clustering core vertices!\n");

    delete[] edge_buf; edge_buf = nullptr;
    delete[] cores; cores = nullptr;
    delete[] bin_head; bin_head = nullptr;
    delete[] bin_next; bin_next = nullptr;

    cluster_noncore_vertices(eps_a2, eps_b2, miu);
}

void Pscan::pSCAN_disjoint(const char* eps_s, int _miu, int* minCN) {
    get_eps(eps_s);
    miu = _miu;

    if (similar_degree == nullptr) similar_degree = new int[n];
    for (ui i = 0; i < n; i++) similar_degree[i] = 1;

    if (effective_degree == nullptr) effective_degree = new int[n];
    for (ui i = 0;i < n;i++) effective_degree[i] = degree[i];

    if (pa == nullptr) pa = new int[n];
    if (rank == nullptr) rank = new int[n];
    for (ui i = 0;i < n;i++) {
        pa[i] = i;
        rank[i] = 0;
    }

    ui* edge_buf = new ui[n];
    int* cores = new int[n];
    int cores_n = 0;

    for (int i = 0; i < m; i++) {
        min_cn[i] = minCN[i];
    }

    prune_and_cross_link_for_disjoint(eps_a2, eps_b2, miu, cores, cores_n);
    //printf("\t*** Finished prune and cross link!\n");

    int* bin_head = new int[n + 1];
    int* bin_next = new int[n];
    for (ui i = 0;i < n + 1;i++) bin_head[i] = -1;

    // 下面这段有什么用？bin_head, bin_next是干什么的？
    int max_ed = 0;
    for (ui i = 0;i < n;i++) if (effective_degree[i] >= miu) {
        int ed = effective_degree[i];
        if (ed > max_ed) max_ed = ed;
        bin_next[i] = bin_head[ed];
        bin_head[ed] = i;
    }

    while (true) {
        int u = -1;
        if (cores_n) u = cores[--cores_n];
        else { // 这个else有什么用？
            while (max_ed >= miu && u == -1) {
                for (int x = bin_head[max_ed];x != -1;) {
                    int tmp = bin_next[x];
                    int ed = effective_degree[x];
                    if (ed == max_ed) {
                        u = x;
                        bin_head[max_ed] = bin_next[x];
                        break;
                    } else if (ed >= miu) {
                        bin_next[x] = bin_head[ed];
                        bin_head[ed] = x;
                    }
                    x = tmp;
                }
                if (u == -1) {
                    bin_head[max_ed] = -1;
                    --max_ed;
                }
            }
        }
        if (u == -1) break;

        int edge_buf_n = 0;
        for (ui j = pstart[u];j < pstart[u + 1];j++) {
            if (min_cn[j] == -2) continue;

            if (similar_degree[u] < miu || find_root(u) != find_root(edges[j])) edge_buf[edge_buf_n++] = j;
        }

        int i = 0;
        while (similar_degree[u] < miu && effective_degree[u] >= miu && i < edge_buf_n) {
            ui idx = edge_buf[i];
            if (min_cn[idx] != -1) {
                int v = edges[idx];

                min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2); // similar_chech_OP only return -1 or -2;

                if (min_cn[idx] == -1) ++similar_degree[u];
                else --effective_degree[u];

                if (effective_degree[v] >= 0) {
                    if (min_cn[idx] == -1) {
                        ++similar_degree[v];

                        if (similar_degree[v] == miu) cores[cores_n++] = v;
                    } else --effective_degree[v];
                }
            }

            ++i;
        }

        effective_degree[u] = -1; // u is a core.

        if (similar_degree[u] < miu) continue;

        for (int j = 0;j < edge_buf_n;j++) {
            ui idx = edge_buf[j];
            if (min_cn[idx] == -1 && similar_degree[edges[idx]] >= miu) my_union(u, edges[idx]);
        }

        while (i < edge_buf_n) {
            ui idx = edge_buf[i];
            int v = edges[idx];
            if (min_cn[idx] < 0 || similar_degree[v] < miu || find_root(u) == find_root(v)) {
                ++i;
                continue;
            }

            min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2);

            if (effective_degree[v] >= 0) { // if < 0 , means it is a core?
                if (min_cn[idx] == -1) {
                    ++similar_degree[v];

                    if (similar_degree[v] == miu) cores[cores_n++] = v;
                } else --effective_degree[v];
            }

            if (min_cn[idx] == -1) my_union(u, v);

            ++i;
        }
        //printf(")\n");
    }
    //printf("\t*** Finished clustering core vertices!\n");

    delete[] edge_buf; edge_buf = nullptr;
    delete[] cores; cores = nullptr;
    delete[] bin_head; bin_head = nullptr;
    delete[] bin_next; bin_next = nullptr;

    cluster_noncore_vertices(eps_a2, eps_b2, miu);

    // getEpsNb();
}

int Pscan::check_common_neighbor(int u, int v, int c) {
    int cn = 2;

    if (degree[u] > degree[v]) swap(u, v);

    int du = degree[u] + 1, dv = degree[v] + 1; // plus 1 is used to make sure considering all common neighbors.
    ui i = pstart[u], j = pstart[v];
    while (i < pstart[u + 1] && j < pstart[v + 1] && cn < c && du >= c && dv >= c) {
        if (edges[i] < edges[j]) {
            --du;
            ++i;
        } else if (edges[i] > edges[j]) {
            --dv;
            ++j;
        } else {
            ++cn;
            ++i;
            ++j;
        }
    }

    if (cn >= c) return -1;
    return -2;
}

int Pscan::similar_check_OP(int u, ui idx, int eps_a2, int eps_b2) {
    int v = edges[idx];

    if (min_cn[idx] == 0) {
        int du = degree[u], dv = degree[v];
        int c = compute_common_neighbor_lowerbound(du, dv, eps_a2, eps_b2);

        if (c <= 2) return -1;

        min_cn[idx] = min_cn[reverse[idx]] = c;
    }

    if (mode == 0) {
        return check_common_neighbor(u, v, min_cn[idx]);
    }

    return check_disjoint_neighbor(u, v, min_cn[idx]);
}

int Pscan::compute_common_neighbor_lowerbound(int du, int dv, int eps_a2, int eps_b2) {
    int c = (int)(sqrtl((((long double)du) * ((long double)dv) * eps_a2) / eps_b2));

    if (((long long)c) * ((long long)c) * eps_b2 < ((long long)du) * ((long long)dv) * eps_a2) ++c;

    return c;
}

void Pscan::prune_and_cross_link(int eps_a2, int eps_b2, int miu, int* cores, int& cores_e) {
    for (ui i = 0;i < n;i++) { //must be iterating from 0 to n-1
        for (ui j = pstart[i];j < pstart[i + 1];j++) {
            if (edges[j] < i) {
                if (min_cn[j] == 0) min_cn[j] = -2;
                continue; //this edge has already been checked
            }

            int v = edges[j];
            int a = degree[i], b = degree[v];
            if (a > b) swap(a, b);

            if (((long long)a) * eps_b2 < ((long long)b) * eps_a2) {
                min_cn[j] = -2;

                --effective_degree[i];
                --effective_degree[v];
            } else {
                int c = compute_common_neighbor_lowerbound(a, b, eps_a2, eps_b2);

                if (c <= 2) {
                    min_cn[j] = -1;

                    ++similar_degree[i];
                    ++similar_degree[v];

                    if (similar_degree[i] == miu) cores[cores_e++] = i;
                    if (similar_degree[v] == miu) cores[cores_e++] = v;
                } else min_cn[j] = c;
            }

            if (min_cn[j] != -2) {
                ui r_id = binary_search(edges, pstart[v], pstart[v + 1], i);
                reverse[j] = r_id;
                reverse[r_id] = j;

                min_cn[r_id] = min_cn[j];
            }
        }
    }
}

void Pscan::prune_and_cross_link_for_disjoint(int eps_a2, int eps_b2, int miu, int* cores, int& cores_e) {
    for (ui i = 0;i < n;i++) { //must be iterating from 0 to n-1
        for (ui j = pstart[i];j < pstart[i + 1];j++) {
            if (edges[j] < i) {
                continue;
            }

            int v = edges[j];
            int a = degree[i], b = degree[v];
            if (a > b) swap(a, b);

            if (min_cn[j] == -2) {
                --effective_degree[i];
                --effective_degree[v];
            } else {
                int c = compute_common_neighbor_lowerbound(a, b, eps_a2, eps_b2);

                if (c <= 2) {
                    min_cn[j] = -1;

                    ++similar_degree[i];
                    ++similar_degree[v];

                    if (similar_degree[i] == miu) {
                        cores[cores_e++] = i;
                    }
                    if (similar_degree[v] == miu) {
                        cores[cores_e++] = v;
                    }
                } else min_cn[j] = c;
            }

            if (min_cn[j] != -2) {
                ui r_id = binary_search(edges, pstart[v], pstart[v + 1], i);
                reverse[j] = r_id;
                reverse[r_id] = j;

                min_cn[r_id] = min_cn[j];
            }
        }
    }
}

int Pscan::find_root(int u) {
    int x = u;
    while (pa[x] != x) x = pa[x];

    while (pa[u] != x) {
        int tmp = pa[u];
        pa[u] = x;
        u = tmp;
    }

    return x;
}

void Pscan::my_union(int u, int v) {
    int ru = find_root(u);
    int rv = find_root(v);

    if (ru == rv) return;

    if (rank[ru] < rank[rv]) pa[ru] = rv;
    else if (rank[ru] > rank[rv]) pa[rv] = ru;
    else {
        pa[rv] = ru;
        ++rank[ru];
    }
}

void Pscan::get_eps(const char* eps_s) {
    int i = 0, eps_a = 0, eps_b = 1;
    while (eps_s[i] != '\0' && eps_s[i] != '.') {
        eps_a = eps_a * 10 + (eps_s[i] - '0');
        ++i;
    }

    if (eps_s[i] == '.') {
        ++i;
        while (eps_s[i] != '\0') {
            eps_a = eps_a * 10 + (eps_s[i] - '0');
            eps_b *= 10;
            ++i;
        }
    }

    if (eps_a > eps_b || eps_b > 100 || eps_a <= 0) {
        printf("??? Wrong eps format: %d/%d, %s\n", eps_a, eps_b, eps_s);
        exit(1);
    }

    eps_a2 = eps_a * eps_a;
    eps_b2 = eps_b * eps_b;
}

FILE* Pscan::open_file(const char* file_name, const char* mode) {
    FILE* f = fopen(file_name, mode);
    if (f == nullptr) {
        printf("Can not open file:%s\n", file_name);
        exit(1);
    }

    return f;
}

int Pscan::check_disjoint_neighbor(int u, int v, int c) {
    vector<int> commonNB;

    ui i = pstart[u], j = pstart[v];
    while (i < pstart[u + 1] && j < pstart[v + 1]) {
        if (edges[i] < edges[j]) {
            ++i;
        } else if (edges[i] > edges[j]) {
            ++j;
        } else {
            commonNB.push_back(index2id[edges[i]]);
            ++i;
            ++j;
        }
    }

    int cn = disjoinNb(commonNB, index2id[u], index2id[v], c);

    if (cn >= c) return -1;
    return -2;
}

int Pscan::disjoinNb(vector<int>& commonNB, int vertexU, int vertexV, int c) {
    int cn = 2;

    int count = -1;

    for (int vertexW : commonNB) {
        count++;
        if (cn >= c || cn + (commonNB.size() - count) < c) {
            return cn;
        }
        if (verifyFalseSet.contains({ vertexW, vertexU, vertexV })) {
            continue;
        }
        if (verifyTrueSet.contains({ vertexW, vertexU, vertexV })) {
            cn++;
            continue;
        }
        MyTuple tuple1 = { vertexU, vertexV, metaPath };
        MyTuple tuple2 = { vertexU, vertexW, metaPath };
        MyTuple tuple3 = { vertexV, vertexW, metaPath };
        vector<MyTuple> lambda = { tuple1, tuple2, tuple3 };
        long long time1 = getTime(start);
        if (verifyExistence(lambda)) {
            cn++;
            verifyTrueSet.insert({ vertexW, vertexU, vertexV });
        } else {
            verifyFalseSet.insert({ vertexW, vertexU, vertexV });
        }
        long long time2 = getTime(start);
        totalVerifyTime += time2 - time1;
    }

    return cn;
}

bool Pscan::verifyExistence(vector<MyTuple>& lambda) {
    vector<set<int>> listOfComNb;

    long long time1 = getTime(start);

    for (MyTuple tup : lambda) {
        long long time3 = getTime(start);
        int pathVLen = tup.metaPath.vertex.size();
        int midIndex = (pathVLen - 1) >> 1;

        // get M(x_i)
        set<int> Mx_i = { tup.vertex1 };
        set<int> temp2;
        for (int i = 1; i <= midIndex; i++) {
            getNB(Mx_i, temp2, tup, i, false);
            Mx_i = temp2;
            temp2.clear();
        }

        // get M(y_i)
        set<int> My_i = { tup.vertex2 };
        for (int i = pathVLen - 2; i >= midIndex; i--) {
            getNB(My_i, temp2, tup, i, true);
            My_i = temp2;
            temp2.clear();
        }

        long long time4 = getTime(start);
        totalTime1 += time4 - time3;

        // get intersection
        time3 = getTime(start);
        set<int> intersection;
        set_intersection(Mx_i.begin(), Mx_i.end(), My_i.begin(), My_i.end(), inserter(intersection, intersection.begin()));
        if (intersection.size() == 0) {
            cout << "Mx_i size: " << Mx_i.size() << endl;
            for (int i : Mx_i) {
                cout << i << " ";
            }
            cout << endl;
            for (int i : My_i) {
                cout << i << " ";
            }
            cout << endl;
            cout << "My_i size: " << My_i.size() << endl;
            cout << "v1: " << tup.vertex1 << "\tv2: " << tup.vertex2 << endl;
        }
        listOfComNb.push_back(intersection);

        time4 = getTime(start);
        totalTime2 += time4 - time3;
    }

    long long time2 = getTime(start);
    totalGetNbTime += time2 - time1;

    // sort the vector<set<int>> in ascending order according to the size of each set.
    // auto compareSetSize = [](const set<int>& set1, const set<int>& set2) {
    //     return set1.size() < set2.size();
    //     };
    // sort(listOfComNb.begin(), listOfComNb.end(), compareSetSize);

    vector<int> LArr; // record the different vertex. i.e. different instance.

    // enumeration and verify.
    return enumeration(listOfComNb, 0, LArr, lambda);
}

bool Pscan::hasSameValue(const vector<int>& arr, int vertex) {
    for (int val : arr) {
        if (val == vertex) {
            return true;
        }
    }
    return false;
}

bool Pscan::enumeration(const vector<set<int>>& listOfComNb, int index, vector<int>& LArr, vector<MyTuple>& lambda) {
    set<int> ComNb = listOfComNb.at(index);
    for (int vex : ComNb) {
        if (hasSameValue(LArr, vex)) {
            continue;
        }
        LArr.push_back(vex);
        if (index < listOfComNb.size() - 1) {
            if (enumeration(listOfComNb, index + 1, LArr, lambda)) {
                // cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
                // cout << "listOfComNb size = " << listOfComNb.size() << endl;
                // for (auto& item : listOfComNb) {
                //     cout << "set size = " << item.size() << endl;
                // }
                // cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
                return true;
            } else {
                LArr.pop_back();
            }
        } else {
            vector<MyTuple> lambda2;
            for (int i = 0; i < lambda.size(); i++) {
                MyTuple element = lambda.at(i);
                int pathVLen = element.metaPath.vertex.size();
                int midIndex = (pathVLen - 1) >> 1;

                // generate l(Pj).
                vector<int> LVertex = { element.metaPath.vertex.at(0) };
                vector<int> LEdge;
                for (int j = 1; j <= midIndex; j++) {
                    LVertex.push_back(element.metaPath.vertex.at(j));
                    LEdge.push_back(element.metaPath.edge.at(j - 1));
                }
                MetaPath LMetaPath(LVertex, LEdge);
                if (LMetaPath.pathLen > 1) {
                    MyTuple LTup = { element.vertex1, LArr[i], LMetaPath };
                    // cout << element.vertex1 << "," << LArr[i] << " : " << LMetaPath.toString() << endl;
                    lambda2.push_back(LTup);
                }

                // generate r(Pj).
                vector<int> RVertex = { element.metaPath.vertex.at(pathVLen - 1) };
                vector<int> REdge;
                for (int j = pathVLen - 2; j >= midIndex; j--) {
                    RVertex.push_back(element.metaPath.vertex.at(j));
                    REdge.push_back(element.metaPath.edge.at(j));
                }
                MetaPath RMetaPath(RVertex, REdge);
                if (RMetaPath.pathLen > 1) {
                    MyTuple RTup = { element.vertex2, LArr[i], RMetaPath };
                    // cout << element.vertex2 << "," << LArr[i] << " : " << RMetaPath.toString() << endl;
                    lambda2.push_back(RTup);
                }
            }
            if (lambda2.empty()) {
                // cout << "find one" << endl;
                return true;
            } else {
                return verifyExistence(lambda2);
            }
        }
    }

    // if (listOfComNb.size() == 6) {
    //     cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    //     cout << "listOfComNb size = " << listOfComNb.size() << endl;
    //     for (auto& item : listOfComNb) {
    //         cout << "set size = " << item.size() << endl;
    //     }
    //     cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    // }

    return false;
}

void Pscan::getNB(set<int>& M_i, set<int>& temp, MyTuple& tup, int index, bool fromRight) {
    for (int vex : M_i) {
        int targetVType = tup.metaPath.vertex.at(index);
        int targetEType;
        int targetEType2; // for the reverse edge.

        if (!fromRight) {
            targetEType = tup.metaPath.edge.at(index - 1);
        } else {
            targetEType = tup.metaPath.edge.at(index);
        }
        targetEType2 = this->edgeReverseMap.at(targetEType);
        vector<int> nbArr = this->hinGraph.at(vex);

        for (int j = 0; j < nbArr.size(); j += 2) {
            int nbVertexID = nbArr[j];
            int nbEdgeID = nbArr[j + 1];
            if (targetVType == vertexType[nbVertexID] && (targetEType == edgeType[nbEdgeID] || targetEType2 == edgeType[nbEdgeID])) {
                temp.insert(nbVertexID);
            }
        }
    }
}

int* Pscan::getMinCN() {
    for (int i = 0; i < m; i++) {
        if (min_cn[i] != -2) {
            min_cn[i] = 0;
        }
    }
    return this->min_cn;
}

void Pscan::showTime() {
    cout << "Total GetNB time: " << totalGetNbTime << endl;
    cout << "Total Enumer time: " << totalVerifyTime - totalGetNbTime << endl;
    cout << "Total Time1: " << totalTime1 << endl;
    cout << "Total Time2: " << totalTime2 << endl;
}

void Pscan::showVerifyTimes() {
    cout << "------------------------------" << endl;
    cout << "verifyTrueSet size: " << verifyTrueSet.size() << endl;
    cout << "verifyFalseSet size: " << verifyFalseSet.size() << endl;
    cout << "verifyTimes: " << verifyTrueSet.size() + verifyFalseSet.size() << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
}
