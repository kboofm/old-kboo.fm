// Generated by CoffeeScript 1.10.0
(function() {
  var bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    extend = function(child, parent) { for (var key in parent) { if (hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; },
    hasProp = {}.hasOwnProperty;

  (function($) {
    return App.Station.ListenNow = (function(superClass) {
      extend(ListenNow, superClass);

      function ListenNow() {
        this.getScrollbarWidth = bind(this.getScrollbarWidth, this);
        this.launch = bind(this.launch, this);
        this.onClick = bind(this.onClick, this);
        return ListenNow.__super__.constructor.apply(this, arguments);
      }

      ListenNow.prototype.button = ".launch-player";

      ListenNow.prototype.maxWidth = 768;

      ListenNow.prototype.maxHeight = 840;

      ListenNow.prototype.popupParams = "toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes";

      ListenNow.prototype.bind = function() {
        this.bindItem('click', this.button, this.onClick);
        return true;
      };

      ListenNow.prototype.onClick = function(event) {
        event.preventDefault();
        this.launch();
        return true;
      };

      ListenNow.prototype.launch = function() {
        var deviceHeight, deviceWidth, params, url;
        deviceWidth = window.screen.width;
        if (deviceWidth > this.maxWidth) {
          deviceWidth = this.maxWidth;
        }
        deviceHeight = window.screen.height;
        if (deviceHeight > this.maxHeight) {
          deviceHeight = this.maxHeight;
        }
        if (deviceWidth === this.maxWidth) {
          deviceWidth += this.getScrollbarWidth();
          deviceHeight -= 18;
          params = "width=" + deviceWidth + ",height=" + deviceHeight + "," + this.popupParams;
        }
        url = "/listen-now";
        window.open(url, "player", params);
        return true;
      };

      ListenNow.prototype.getScrollbarWidth = function() {
        var div, width;
        div = document.createElement("div");
        div.innerHTML = "<div style=\"width:50px;height:50px;position:absolute;left:-50px;top:-50px;overflow:auto;\">\n  <div style=\"width:1px;height:100px;\"></div>\n</div>";
        div = div.firstChild;
        document.body.appendChild(div);
        width = div.offsetWidth - div.clientWidth;
        document.body.removeChild(div);
        return width + 20;
      };

      return ListenNow;

    })(C4.Components.Base);
  })(jQuery);

}).call(this);