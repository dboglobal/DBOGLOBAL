CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 2;
	rq = 0;
	tid = 559;
	title = 55902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 55907;
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
				minlvl = 19;
			}
			CDboTSClickNPC
			{
				npcidx = "4111104;";
			}
			CDboTSCheckClrQst
			{
				and = "558;";
				flink = 1;
				flinknextqid = "577;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 100;
			prelnk = "7;";

			CDboTSActWorldPlayScript
			{
				sid = 559;
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
				sid = 559;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSRcvSvrEvt
			{
				id = 71;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 9;
			prelnk = "5;";

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
			nextlnk = 5;
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

			CDboTSActNPCConv
			{
				conv = 55909;
				ctype = 1;
				idx = 4111104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 55908;
				gtype = 3;
				area = 55901;
				goal = 55904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 55905;
				stype = 2;
				taid = 1;
				title = 55902;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 55914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 55901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "9;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4111104;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 55908;
			gtype = 3;
			oklnk = 2;
			area = 55901;
			goal = 55904;
			sort = 55905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 55902;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;4;2;";
		}
	}
	CNtlTSGroup
	{
		gid = 12;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 55907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckNPCDead
			{
				idxlist = "1781201;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 11;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 55907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckDistWithNPC
			{
				dist = "60.000000";
				idxlist = "1781201;";
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
	}
}

