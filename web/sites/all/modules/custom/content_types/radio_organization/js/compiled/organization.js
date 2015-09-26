// Generated by CoffeeScript 1.9.3
(function() {
  var bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    extend = function(child, parent) { for (var key in parent) { if (hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; },
    hasProp = {}.hasOwnProperty;

  (function($) {
    App.Modules.Organization = (function(superClass) {
      extend(Organization, superClass);

      function Organization() {
        this.venueCheckboxChange = bind(this.venueCheckboxChange, this);
        return Organization.__super__.constructor.apply(this, arguments);
      }

      Organization.prototype.divOrganizationType = "#edit-field-organization-type-und";

      Organization.prototype.verticalTabs = ".vertical-tab-button";

      Organization.prototype.init = function() {
        Organization.__super__.init.call(this);
        this.$venueTab = $(this.verticalTabs).find("a:contains('Venue')").parent();
        this.$venueCheckbox = $(this.divOrganizationType).find("label:contains('Venue')").parent().find("input");
        this.toggleVenueTab();
        return true;
      };

      Organization.prototype.bind = function() {
        this.bindToObject("change", this.$venueCheckbox, this.venueCheckboxChange);
        return true;
      };

      Organization.prototype.toggleVenueTab = function() {
        if (this.$venueCheckbox[0].checked) {
          this.$venueTab.show();
        } else {
          this.$venueTab.hide();
        }
        return true;
      };

      Organization.prototype.venueCheckboxChange = function(event) {
        this.toggleVenueTab();
        return true;
      };

      return Organization;

    })(C4.Components.Base);
    return $(function() {
      new App.Modules.Organization();
      return true;
    });
  })(jQuery);

}).call(this);