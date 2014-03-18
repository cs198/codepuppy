angular.module('codepuppy').controller('LeaderCourseCtrl', ['$scope', '$routeParams', '$http', function($scope, $routeParams, $http) {
    $scope.message = 'Look! I am a course page for ' + $routeParams.courseID;

	$scope.upcomingAssignments = {'title': 'Upcoming Assignments', 'elements': []};
    $scope.recentAssignments = {'title': 'Recent Assignments', 'elements': []};

    var getAssignments = function(courseID) {
    $http({method: 'GET', url: '/courses/' + courseID + '/assignments.json'})
    .success(function(data, status, headers, config) {
		var now = moment();
		for (var i = 0; i < data.length; ++i) {
			var dueTime = moment(data[i].date_due);
			data[i].date_due_formatted = dueTime.format('MMM DD, YYYY, hh:mm A');
			data[i].url = '/#/courses/' + $routeParams.courseID + '/assignments/' + data[i].id;
			if(dueTime < now) {
				$scope.recentAssignments.elements.push(data[i]);
			} else {
				$scope.upcomingAssignments.elements.push(data[i]);
			}
		}		
    });
	};

	getAssignments($routeParams.courseID);
}]);