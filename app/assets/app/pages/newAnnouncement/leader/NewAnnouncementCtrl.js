angular.module('codepuppy').controller('NewAnnouncementCtrl', ['$scope', '$routeParams', function($scope, $routeParams) {
	$scope.formError = false;

    $scope.submit = function() {
		if($scope.announcement.$valid) {
			$scope.formError = false;
			// POST
		} else {
			$scope.formError = true;
		}
    };

    $scope.cancel = function() {
		// Redirect
    };
}]);