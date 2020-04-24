CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 2;
	rq = 0;
	tid = 551;
	title = 55102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 551;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 551;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 12;

			CDboTSRcvSvrEvt
			{
				id = 75;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActETimerE
			{
				taeid = 1;
				taid = 1;
				tceid = 4;
			}
			CDboTSActETimerE
			{
				taeid = 2;
				taid = 2;
				tceid = 4;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActETimerS
			{
				sort = 1;
				taid = 1;
				tgid = 11;
				time = 2000;
			}
			CDboTSActETimerS
			{
				sort = 1;
				taid = 2;
				tgid = 12;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 55108;
				gtype = 3;
				area = 55101;
				goal = 55104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 55105;
				stype = 2;
				taid = 1;
				title = 55102;
			}
			CDboTSActNPCConv
			{
				conv = 55109;
				ctype = 1;
				idx = 2611101;
				taid = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 55114;
			nextlnk = 13;
			rwdexp = 0;
			rwdtbl = 55101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "12;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "13;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;4;2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 55108;
			gtype = 3;
			oklnk = 2;
			area = 55101;
			goal = 55104;
			sort = 55105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 55102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 55107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 32;
			}
			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSCheckClrQst
			{
				and = "550;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2611101;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 11;

		CDboTSContGAct
		{
			cid = 1;
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 55107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckDistWithNPC
			{
				dist = "50.000000";
				idxlist = "2611203;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 12;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 55107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckNPCDead
			{
				idxlist = "2611203;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
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
	}
}

