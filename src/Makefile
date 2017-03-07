
PLUGINS_DIR = ./plugins

STRADE_SHARE = strade_share
STRADE_SHARE_LOGIC = strade_share_logic
CANDLESTICK = candlestick
REALINFO = realinfo
REALTIME = realtime
USER = user

MAKE = make
CLEAN = make clean

all: strade_share strade_share_logic candlestick realinfo realtime user

strade_share:
	cd $(PLUGINS_DIR)/$(STRADE_SHARE)&&$(CLEAN)&&$(MAKE)

strade_share_logic:
	cd $(PLUGINS_DIR)/$(STRADE_SHARE_LOGIC)&&$(CLEAN)&&$(MAKE)

candlestick:
	cd $(PLUGINS_DIR)/$(CANDLESTICK)&&$(CLEAN)&&$(MAKE)

realinfo:
	cd $(PLUGINS_DIR)/$(REALINFO)&&$(CLEAN)&&$(MAKE)

realtime:
	cd $(PLUGINS_DIR)/$(REALTIME)&&$(CLEAN)&&$(MAKE)

user:
	cd $(PLUGINS_DIR)/$(USER)&&$(CLEAN)&&$(MAKE)
