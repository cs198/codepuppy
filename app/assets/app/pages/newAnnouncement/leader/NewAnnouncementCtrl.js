angular.module('codepuppy').controller('NewAnnouncementCtrl', ['$scope', '$stateParams', '$location', '$http', function($scope, $stateParams, $location, $http) {
	$scope.formError = false;

    $scope.submit = function() {
		if($scope.announcement.$valid) {
			var createAnnouncement = function() {
              var urlParams = {
                course_id: $stateParams.courseID,
                subject: $scope.subject,
                body: $scope.body
              };

              $http({
                method: 'POST',
                url: '/courses/' + $stateParams.courseID + '/announcements.json',
                data: urlParams
              }).success(function(data, status, headers, config) {
				var path = '/courses/' + $stateParams.courseID;
				$location.path(path);
              });
            };

            createAnnouncement();
		} else {
			$scope.formError = true;
		}
    };

    $scope.cancel = function() {
		var path = '/courses/' + $stateParams.courseID;
		$location.path(path);
    };
}]);
