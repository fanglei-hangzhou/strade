#result = ts.get_k_data('399300', index=True,start='2016-10-01', end='2016-10-31')

PLUGINS_DIR = ../plugins

STRADE_SHARE = strade_share
STRADE_SHARE_LOGIC = strade_share_logic

MAKE = make

all: strade_share

strade_share:
	cd $(PLUGINS_DIR)/$(STRADE_SHARE)&&$(MAKE)

strade_share_logic:
	cd $(PLUGINS_DIR)/$(STRADE_SHARE_LOGIC)&&$(MAKE)

