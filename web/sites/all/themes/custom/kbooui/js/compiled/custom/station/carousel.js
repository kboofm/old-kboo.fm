// Generated by CoffeeScript 1.10.0
(function() {
  var bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    extend = function(child, parent) { for (var key in parent) { if (hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; },
    hasProp = {}.hasOwnProperty;

  (function($) {
    App.Station.Carousel = (function(superClass) {
      extend(Carousel, superClass);

      function Carousel() {
        this.renderWeek = bind(this.renderWeek, this);
        this.renderDay = bind(this.renderDay, this);
        this.renderEpisode = bind(this.renderEpisode, this);
        this.renderSchedule = bind(this.renderSchedule, this);
        this.renderToolbar = bind(this.renderToolbar, this);
        this.getCarousel = bind(this.getCarousel, this);
        this.change = bind(this.change, this);
        this.prev = bind(this.prev, this);
        this.next = bind(this.next, this);
        return Carousel.__super__.constructor.apply(this, arguments);
      }

      Carousel.prototype.nextButton = ".schedule-carousel-next .schedule-trigger";

      Carousel.prototype.prevButton = ".schedule-carousel-prev .schedule-trigger";

      Carousel.prototype.carousel_timestamp = ".schedule-carousel-timestamp";

      Carousel.prototype.$carousel = null;

      Carousel.prototype.$toolbar = null;

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

      Carousel.prototype.change = function(event, direction) {
        var route;
        this.getCarousel(event);
        route = "/api/schedule/" + this.type + "/" + this.stream + "/" + direction + "/" + this.timestamp;
        jQuery.get(route, this.renderSchedule);
        return true;
      };

      Carousel.prototype.getCarousel = function(event) {
        var $button, carousel_id;
        $button = $(event.target).parent();
        this.$toolbar = $button.parent();
        carousel_id = $button.data("carousel");
        this.$carousel = $("#" + carousel_id);
        this.timestamp = this.$carousel.find(this.carousel_timestamp).attr("data-timestamp");
        this.type = this.$carousel.attr("data-type");
        this.stream = this.$carousel.attr("data-stream");
        return true;
      };

      Carousel.prototype.dataItem = function(item) {
        return {
          "program": {
            "text": item['title'],
            "href": item['url']
          },
          "formatted-date": item['start']['formatted_date'],
          "formatted-time": {
            "start-time": item['start']['formatted_time'],
            "finish-time": item['finish']['formatted_time']
          }
        };
      };

      Carousel.prototype.getDirectives = function() {
        return {
          "schedule-carousel-timestamp": {
            "data-timestamp": function() {
              return "" + this.timestamp;
            }
          },
          "schedule-item": {
            "title-link": {
              "text": function() {
                return this.program.text;
              },
              "href": function() {
                return this.program.href;
              }
            }
          }
        };
      };

      Carousel.prototype.renderToolbar = function(start, showTime) {
        var data, datetime;
        if (showTime == null) {
          showTime = false;
        }
        datetime = start['formatted_date'];
        if (showTime) {
          datetime = datetime + " " + start['formatted_time'];
        }
        data = {
          datetime: datetime
        };
        return this.$toolbar.render(data);
      };

      Carousel.prototype.renderSchedule = function(response) {
        if (response.length === 0) {
          return;
        }
        if (this.type === "episode") {
          this.renderEpisode(response);
        }
        if (this.type === "day") {
          this.renderDay(response);
        }
        if (this.type === "week") {
          this.renderWeek(response);
        }
        return true;
      };

      Carousel.prototype.renderEpisode = function(response) {
        var data, start;
        start = response[0]['start'];
        data = {
          timestamp: start['timestamp'],
          "schedule-item": [this.dataItem(response[0])]
        };
        this.$carousel.render(data, this.getDirectives());
        this.renderToolbar(start, true);
        return true;
      };

      Carousel.prototype.renderDay = function(response) {
        var data, item, start;
        start = response[0]['start'];
        data = {
          timestamp: start['timestamp'],
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
        this.$carousel.find('.cull').remove();
        this.$carousel.render(data, this.getDirectives());
        this.renderToolbar(start);
        return true;
      };

      Carousel.prototype.renderWeek = function(response) {
        var container, item, week_start, weekday, weekdays;
        week_start = null;
        weekdays = [];
        for (weekday in response) {
          if (week_start == null) {
            week_start = response[weekday][0]['start'];
          }
          weekdays.push({
            "schedule-dayofweek": weekday,
            "schedule-items": (function() {
              var i, len, ref, results;
              ref = response[weekday];
              results = [];
              for (i = 0, len = ref.length; i < len; i++) {
                item = ref[i];
                results.push(this.dataItem(item));
              }
              return results;
            }).call(this)
          });
        }
        container = {
          weekdays: weekdays,
          timestamp: week_start['timestamp']
        };
        this.$carousel.find('.cull').remove();
        this.$carousel.render(container, this.getDirectives());
        this.renderToolbar(week_start);
        return true;
      };

      return Carousel;

    })(C4.Components.Base);
    return $(function() {
      new App.Station.Carousel();
      return true;
    });
  })(jQuery);

}).call(this);
