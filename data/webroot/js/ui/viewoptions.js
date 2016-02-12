define(["jquery", "api/viewoptions", "api/actions", "api/properties", "jquery-ui"], function($, viewOptionApi, actionApi, propApi) {
	"use strict";

	var $vo_projectionlist;
	var $vo_landscapelist;
	var $vo_skyculturelist;

	function fillProjectionList(data) {
		var $projectionlist = $("#vo_projectionlist");
		$projectionlist.empty();
		var prev = $projectionlist.prev();
		$projectionlist.detach();

		for (var val in data) {
			var option = document.createElement("option");
			option.value = val;
			option.textContent = data[val];
			$projectionlist.append(option);
		}

		prev.after($projectionlist);
	}

	function fillLandscapeList(data) {
		var sortable = [];
		var val;
		for (val in data)
			sortable.push([val, data[val]]);

		sortable.sort(function(a, b) {
			if (a[1] < b[1])
				return -1;
			if (a[1] > b[1])
				return 1;
			return 0;
		});

		$vo_landscapelist.empty();

		sortable.forEach(function(val) {
			var option = document.createElement("option");
			option.value = val[0];
			option.textContent = val[1];
			$vo_landscapelist.append(option);
		});
	}

	function fillSkycultureList(data) {
		var sortable = [];
		var val;
		for (val in data)
			sortable.push([val, data[val]]);

		sortable.sort(function(a, b) {
			if (a[1] < b[1])
				return -1;
			if (a[1] > b[1])
				return 1;
			return 0;
		});

		$vo_skyculturelist.empty();

		sortable.forEach(function(val) {
			var option = document.createElement("option");
			option.value = val[0];
			option.textContent = val[1];
			$vo_skyculturelist.append(option);
		});
	}

	function initControls() {
		$vo_landscapelist = $("#vo_landscapelist");
		$vo_skyculturelist = $("#vo_skyculturelist");
		// Maybe its enough/better to try and connect to vo_ divs, not the tabs? The vo_ are the better address when they move around different layouts.
		actionApi.connectActionContainer($("#tab_view"));
		actionApi.connectActionContainer($("#tab_view_stars"));
		actionApi.connectActionContainer($("#tab_view_planets"));
		actionApi.connectActionContainer($("#tab_view_atm"));
		actionApi.connectActionContainer($("#tab_view_grids"));
		actionApi.connectActionContainer($("#tab_landscape"));
		actionApi.connectActionContainer($("#tab_skyculture"));
		$vo_projectionlist = $("#vo_projectionlist");
		$vo_projectionlist.change(function(event) {	
			if (this.selectedIndex >= 0) {
				var proj = this.options[this.selectedIndex].value;
				viewOptionApi.setProjection(proj);
			}
		});

		$vo_landscapelist.change(function(event) {
			if (this.selectedIndex >= 0) {
				var ls = this.options[this.selectedIndex].value;
				viewOptionApi.setLandscape(ls);
			}
		});

		$vo_skyculturelist.change(function(evt) {
			if (this.selectedIndex >= 0) {
				var sc = this.options[this.selectedIndex].value;
				viewOptionApi.setSkyculture(sc);
			}
		});

		actionApi.connectCheckbox($("#vo_dsotypefiltercheckbox"));

		viewOptionApi.loadProjectionList(fillProjectionList);
		viewOptionApi.loadLandscapeList(fillLandscapeList);
		viewOptionApi.loadSkycultureList(fillSkycultureList);

		viewOptionApi.registerCatalogFlags($("#vo_dsocatalog"));
		viewOptionApi.registerTypeFlags($("#vo_dsotype > div")); //needs a stricter selector to prevent capturing the header checkbox
	}

	$(actionApi).on("stelActionChanged:actionShow_LightPollution_Database", function(evt,data){
		$("#atmosphere_bortlescaleindex").spinner("option", "disabled", data.isChecked);
	});

	$(propApi).on("stelPropertyChanged:prop_LandscapeMgr_flagLandscapeUseMinimalBrightness", function(evt,data){
		$("#landscape_defaultMinimalBrightness").spinner("option", "disabled", !data.value);
		$("#landscape_flagLandscapeSetsMinimalBrightness").prop("disabled", !data.value);
	});

	$(actionApi).on("stelActionChanged:actionSet_Nebula_TypeFilterUsage", function(evt, data) {
		$("#vo_dsotype > div input[type='checkbox']").prop("disabled", !data.isChecked);
	});


	$(viewOptionApi).on("projectionChanged", function(evt, proj) {
		//this forces a reload of the iframe
		$("#vo_projectioninfo").attr("src", function(i, val) {
			return val;
		});

		$vo_projectionlist.val(proj);
	});

	$(viewOptionApi).on("landscapeChanged", function(evt, landscape) {
		//this forces a reload of the iframe
		$("#vo_landscapeinfo").attr('src', function(i, val) {
			return val;
		});

		$vo_landscapelist.val(landscape);
	});

	$(viewOptionApi).on("skycultureChanged", function(evt, skyculture) {
		//this forces a reload of the iframe
		$("#vo_skycultureinfo").attr('src', function(i, val) {
			return val;
		});

		$vo_skyculturelist.val(skyculture);
	});

	$(initControls);
});