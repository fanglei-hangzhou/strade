#include "src/candlestick_singlestock.h"
#include "src/candlestick_compositeindex.h"
#include "stdio.h"
static STOCK_HISTORY_MAP build_test_info() {
  STOCK_HISTORY_MAP info;
  StockHistoryInfo a1("2016-12-12", 400, 20, 30, 90, 1);
  info["2016-12-12"] = a1;
  StockHistoryInfo a2("2016-12-13", 200, 30, 10, 50, 2);
  info["2016-12-13"] = a2;
  StockHistoryInfo a3("2016-12-14", 201, 40, 10, 50, 2);
  info["2016-12-14"] = a3;
  StockHistoryInfo a4("2016-12-15", 200, 30, 10, 50, 8);
  info["2016-12-15"] = a4;

  if (1) {
  StockHistoryInfo a1("2016-11-03", 100, 40, 30, 90, 1);
  info["2016-11-03"] = a1;
  StockHistoryInfo a2("2016-11-15", 200, 30, 60, 50, 6);
  info["2016-11-15"] = a2;
  StockHistoryInfo a3("2016-11-14", 201, 23, 60, 50, 6);
  info["2016-11-14"] = a3;
  StockHistoryInfo a4("2016-11-25", 200, 30, 60, 50, 8);
  info["2016-11-25"] = a4;
  }
  return info;
}

static void test_month() {
  printf("monthtest---------------------\n");
  std::list<SingleStockInfo> ll;
  SingleStockCandleStickData::GetInstance()->LoadSingleStockHisData(
      "000000", STOCK_HISTORY_MONTH, build_test_info());
  printf("dklsfjdlsfj\n");
  SingleStockCandleStickData::GetInstance()->GetSingleStockHisData(
      "000000", STOCK_HISTORY_MONTH, &ll);
  printf("%d\n", ll.size());
  if (ll.size() == 0)
    return;
  std::list<SingleStockInfo>::iterator it = ll.begin();
  while (it != ll.end()) {
    printf("high %.2f low %.2f  amount %ld date %s\n", it->get_high_price(),
           it->get_low_price(), it->get_amount(), it->get_date().c_str());
    it++;
  }
}

static void test_day() {
  printf("daytest---------------------\n");
  std::list<SingleStockInfo> ll;
  SingleStockCandleStickData::GetInstance()->LoadSingleStockHisData(
      "000000", build_test_info());
  printf("dklsfjdlsfj\n");
  SingleStockCandleStickData::GetInstance()->GetSingleStockHisData(
      "000000", STOCK_HISTORY_DAY, &ll);
  printf("%d\n", ll.size());
  if (ll.size() == 0)
    return;
  std::list<SingleStockInfo>::iterator it = ll.begin();
  while (it != ll.end()) {
    printf("high %.2f low %.2f  amount %ld date %s\n", it->get_high_price(),
           it->get_low_price(), it->get_amount(), it->get_date().c_str());
    it++;
  }
}

static void test_day_com() {
  printf("daytest---------------------\n");
  std::list<CompositeindexInfo> ll;
  CompositeindexCandleStickData::GetInstance()->LoadCompositeindexHisData(
      STOCK_HISTORY_DAY, build_test_info());
  printf("dklsfjdlsfj\n");
  CompositeindexCandleStickData::GetInstance()->GetCompositeindexHisData(
       STOCK_HISTORY_DAY, &ll);
  printf("%d\n", ll.size());
  if (ll.size() == 0)
    return;
  std::list<CompositeindexInfo>::iterator it = ll.begin();
  while (it != ll.end()) {
    printf("high %.2f low %.2f  amount %ld date %s\n", it->get_high_index(),
           it->get_low_index(), it->get_amount(), it->get_date().c_str());
    it++;
  }
}

static void test_week() {
  printf("weektest---------------------\n");
  std::list<SingleStockInfo> ll;
  SingleStockCandleStickData::GetInstance()->LoadSingleStockHisData(
      "000000", STOCK_HISTORY_WEEK, build_test_info());
  printf("dklsfjdlsfj\n");
  SingleStockCandleStickData::GetInstance()->GetSingleStockHisData(
      "000000", STOCK_HISTORY_WEEK, &ll);
  printf("%d\n", ll.size());
  if (ll.size() == 0)
    return;
  std::list<SingleStockInfo>::iterator it = ll.begin();
  while (it != ll.end()) {
    printf("high %.2f low %.2f  amount %ld date %s\n", it->get_high_price(),
           it->get_low_price(), it->get_amount(), it->get_date().c_str());
    it++;
  }
}

int main() {
  test_day_com();
  return 0;
}
