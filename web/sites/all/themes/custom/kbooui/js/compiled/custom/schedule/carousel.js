// Generated by CoffeeScript 1.9.3
(function() {
  var bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    extend = function(child, parent) { for (var key in parent) { if (hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; },
    hasProp = {}.hasOwnProperty;

  window.App.Schedule = window.App.Schedule || {};

  (function($) {
    return App.Schedule.Carousel = (function(superClass) {
      extend(Carousel, superClass);

      function Carousel() {
        this.renderWeek = bind(this.renderWeek, this);
        this.renderDay = bind(this.renderDay, this);
        this.renderEpisode = bind(this.renderEpisode, this);
        this.getWeek = bind(this.getWeek, this);
        this.getDay = bind(this.getDay, this);
        this.getEpisode = bind(this.getEpisode, this);
        this.change = bind(this.change, this);
        this.getCarousel = bind(this.getCarousel, this);
        this.prev = bind(this.prev, this);
        this.next = bind(this.next, this);
        return Carousel.__super__.constructor.apply(this, arguments);
      }

      Carousel.prototype.nextButton = ".schedule-carousel-next .schedule-trigger";

      Carousel.prototype.prevButton = ".schedule-carousel-prev .schedule-trigger";

      Carousel.prototype.carousel_timestamp = ".schedule-carousel-timestamp";

      Carousel.prototype.$carousel = null;

      Carousel.prototype.stream = null;

      Carousel.prototype.type = null;

      Carousel.prototype.timestamp = null;

      Carousel.prototype.bind = function() {
        this.bindItem("click", this.nextButton, this.next);
        this.bindItem("click", this.prevButton, this.prev);
        return true;
      };

      Carousel.prototype.next = function(event) {
        this.change(event, "next");
        return true;
      };

      Carousel.prototype.prev = function(event) {
        this.change(event, "prev");
        return true;
      };

      Carousel.prototype.getCarousel = function(event) {
        var $button, carousel_id;
        $button = $(event.target).parent();
        carousel_id = $button.data("carousel");
        this.$carousel = $("#" + carousel_id);
        this.timestamp = this.$carousel.find(this.carousel_timestamp).attr("data-timestamp");
        this.type = this.$carousel.attr("data-type");
        this.stream = this.$carousel.attr("data-stream");
        this.stream = encodeURIComponent(this.stream);
        return true;
      };

      Carousel.prototype.change = function(event, direction) {
        var route;
        this.getCarousel(event);
        route = "/station/" + this.type + "/" + this.stream + "/" + direction + "/" + this.timestamp;
        if (this.type === 'episode') {
          this.getEpisode(route);
        }
        if (this.type === 'day') {
          this.getDay(route);
        }
        if (this.type === 'week') {
          this.getWeek(route);
        }
        return true;
      };

      Carousel.prototype.getEpisode = function(route) {
        $.get(route, this.renderEpisode);
        return true;
      };

      Carousel.prototype.getDay = function(route) {
        $.get(route, this.renderDay);
        return true;
      };

      Carousel.prototype.getWeek = function(route) {
        $.get(route, this.renderWeek);
        return true;
      };

      Carousel.prototype.dataItem = function(item) {
        var data_item;
        data_item = {
          "title-link": item['title'],
          "formatted-date": item['start']['formatted_date'],
          "formatted-time": {
            "start-time": item['start']['formatted_time'],
            "finish-time": item['finish']['formatted_time']
          }
        };
        return data_item;
      };

      Carousel.prototype.renderEpisode = function(response) {
        var data, directives;
        if (response.length === 0) {
          return;
        }
        data = {
          timestamp: response[0]['start']['timestamp'],
          "schedule-item": [this.dataItem(response[0])]
        };
        directives = {
          "schedule-carousel-timestamp": {
            "data-timestamp": function() {
              return "" + this.timestamp;
            }
          }
        };
        this.$carousel.render(data, directives);
        return true;
      };

      Carousel.prototype.renderDay = function(response) {
        var data, directives, item;
        if (response.length === 0) {
          return;
        }
        data = {
          timestamp: response[0]['start']['timestamp'],
          "schedule-item": (function() {
            var i, len, results;
            results = [];
            for (i = 0, len = response.length; i < len; i++) {
              item = response[i];
              results.push(this.dataItem(item));
            }
            return results;
          }).call(this)
        };
        directives = {
          "schedule-carousel-timestamp": {
            "data-timestamp": function() {
              return "" + this.timestamp;
            }
          }
        };
        this.$carousel.find('.cull').remove();
        this.$carousel.render(data, directives);
        return true;
      };

      Carousel.prototype.renderWeek = function(response) {
        var container, directives, item, timestamp, week_start, weekdata, weekday, weekdays;
        if (response.length === 0) {
          return;
        }
        week_start = null;
        weekdays = [];
        for (weekday in response) {
          timestamp = response[weekday][0]['start']['timestamp'];
          if (!week_start) {
            week_start = timestamp;
          }
          weekdata = {
            "schedule-dayofweek": weekday,
            "schedule-item": (function() {
              var i, len, ref, results;
              ref = response[weekday];
              results = [];
              for (i = 0, len = ref.length; i < len; i++) {
                item = ref[i];
                results.push(this.dataItem(item));
              }
              return results;
            }).call(this)
          };
          weekdays.push(weekdata);
        }
        container = {
          weekdays: weekdays,
          timestamp: week_start
        };
        directives = {
          "schedule-carousel-timestamp": {
            "data-timestamp": function() {
              return "" + this.timestamp;
            }
          }
        };
        this.$carousel.find('.cull').remove();
        this.$carousel.render(container, directives);
        return true;
      };

      return Carousel;

    })(C4.Components.Base);
  })(jQuery);

}).call(this);
