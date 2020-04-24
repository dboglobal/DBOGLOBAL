CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 1;
	tid = 6281;
	title = 628102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 628120;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActETimerE
			{
				taeid = 1;
				taid = 1;
				tceid = 4;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActETimerS
			{
				sort = 1;
				taid = 1;
				tgid = 11;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cd0 = 92041;
				eitype = 5;
				idx0 = 628101;
				cnt0 = 0;
				ectype = 0;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 628108;
				gtype = 1;
				area = 628101;
				goal = 628104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 628105;
				stype = 1;
				taid = 1;
				title = 628102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 628107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 483;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "8;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 484;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 628108;
			gtype = 1;
			oklnk = 2;
			area = 628101;
			goal = 628104;
			sort = 628105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 628102;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cd0 = 92041;
				eitype = 5;
				idx0 = 628101;
				cnt0 = 1;
				ectype = 0;
				etype = 0;
				qtidx = 628130;
				taid = 1;
				esctype = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 11;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 628107;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckOutRGN
			{
				fr = 60;
				fx = 199;
				fz = 343;
				widx = 715000;
			}
		}
	}
}

