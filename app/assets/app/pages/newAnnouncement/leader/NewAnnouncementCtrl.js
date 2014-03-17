angular.module('codepuppy').controller('NewAnnouncementCtrl', ['$scope', '$routeParams', '$location', '$http', function($scope, $routeParams, $location, $http) {
	$scope.formError = false;

    $scope.submit = function() {
		if($scope.announcement.$valid) {
			var createAnnouncement = function() {
              var urlParams = {
                course_id: $routeParams.courseID,
                subject: $scope.subject,
                body: $scope.body
              };

              $http({
                method: 'POST',
                url: '/courses/' + $routeParams.courseID + '/announcements.json',
                data: urlParams
              }).success(function(data, status, headers, config) {
				var path = '/courses/' + $routeParams.courseID;
				$location.path(path);
              });  
            };

            createAnnouncement();
		} else {
			$scope.formError = true;
		}
    };

    $scope.cancel = function() {
		var path = '/courses/' + $routeParams.courseID;
		$location.path(path);
    };
}]);