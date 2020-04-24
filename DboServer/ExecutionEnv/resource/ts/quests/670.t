CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 0;
	tid = 670;
	title = 67002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 6;
			prelnk = "2;";

			CDboTSActETimerS
			{
				sort = 1;
				taid = 2;
				tgid = 12;
				time = 2000;
			}
			CDboTSActETimerS
			{
				sort = 1;
				taid = 1;
				tgid = 11;
				time = 2000;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 15;

			CDboTSRcvSvrEvt
			{
				id = 55;
			}
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActETimerE
			{
				taeid = 1;
				taid = 1;
				tceid = 5;
			}
			CDboTSActWorldPlayScript
			{
				sid = 670;
				start = 0;
				taid = 3;
				uof = 0;
			}
			CDboTSActETimerE
			{
				taeid = 2;
				taid = 2;
				tceid = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActWorldPlayScript
			{
				sid = 670;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 67008;
				gtype = 3;
				area = 67001;
				goal = 67004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 67005;
				stype = 2;
				taid = 1;
				title = 67002;
			}
			CDboTSActNPCConv
			{
				conv = 67009;
				ctype = 1;
				idx = 4151106;
				taid = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 67008;
			gtype = 3;
			oklnk = 2;
			area = 67001;
			goal = 67004;
			sort = 67005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 67002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 67014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 67001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "15;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4141101;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "5;2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 67007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "4151106;";
			}
			CDboTSCheckClrQst
			{
				and = "669;";
				flink = 1;
				flinknextqid = "671;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
	CNtlTSGroup
	{
		gid = 12;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 67007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckNPCDead
			{
				idxlist = "4151106;";
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
	CNtlTSGroup
	{
		gid = 11;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 67007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckDistWithNPC
			{
				dist = "30.000000";
				idxlist = "4151106;";
			}
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

