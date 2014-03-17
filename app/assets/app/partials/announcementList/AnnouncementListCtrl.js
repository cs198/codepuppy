angular.module('codepuppy').controller('AnnouncementListCtrl', ['$scope', '$routeParams', '$http', function($scope, $routeParams, $http) {
	$scope.sortByField = "created_at";
	$scope.reverse = true;
	$scope.newAnnouncementPath = '/#/courses/' + $routeParams.courseID + '/newAnnouncement';
	$scope.selectedAnnouncement = null;
	$scope.announcements = [];

	var getAnnouncements = function() {
    $http({method: 'GET', url: '/courses/' + $routeParams.courseID + '/announcements.json'})
		.success(function(data, status, headers, config) {
			for(var i = 0; i < data.length; ++i) {
				$scope.announcements.push(data[i]);
			}
		});
	};

	getAnnouncements();

	$scope.announcementSelected = function(announcement) {
		$scope.selectedAnnouncement = announcement;
	};
}]);