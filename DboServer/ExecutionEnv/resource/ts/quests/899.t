CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 899;
	title = 89902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "3188102;";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActETimerE
			{
				taeid = 3;
				taid = 1;
				tceid = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 89908;
			gtype = 2;
			oklnk = 2;
			area = 89901;
			goal = 89904;
			sort = 89905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 89902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 89914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 89901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 89907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckBudokaiState
			{
				md = 1;
				st = 0;
			}
			CDboTSCheckLvl
			{
				maxlvl = 29;
				minlvl = 20;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "900;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1311208;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 89909;
				ctype = 1;
				idx = 7071104;
				taid = 2;
			}
			CDboTSActETimerS
			{
				sort = 1;
				taid = 3;
				tgid = 11;
				time = 10000;
			}
			CDboTSActRegQInfo
			{
				cont = 89908;
				gtype = 2;
				area = 89901;
				goal = 89904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 89905;
				stype = 1;
				taid = 1;
				title = 89902;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 11;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckBudokaiState
			{
				md = 1;
				st = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 253;
			prelnk = "1;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
	}
}

