CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 1059;
	title = 105902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 100;
			prelnk = "9;";

			CDboTSActWorldPlayScript
			{
				sid = 1059;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "4;";

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
		CDboTSContGCond
		{
			cid = 8;
			prelnk = "7;";
			nolnk = 255;
			rm = 0;
			yeslnk = 9;

			CDboTSRcvSvrEvt
			{
				id = 239;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;";

			CDboTSActETimerE
			{
				taeid = 1;
				taid = 1;
				tceid = 7;
			}
			CDboTSActETimerE
			{
				taeid = 2;
				taid = 2;
				tceid = 7;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 7;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 1059;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 105909;
				ctype = 1;
				idx = 4191121;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 105908;
				gtype = 3;
				area = 105901;
				goal = 105904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 105905;
				stype = 2;
				taid = 1;
				title = 105902;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 105914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 105901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "10;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7091103;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 105907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSClickNPC
			{
				npcidx = "4191121;";
			}
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
			prelnk = "4;2;";
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
			cont = 105908;
			gtype = 3;
			oklnk = 2;
			area = 105901;
			goal = 105904;
			sort = 105905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 105902;
		}
	}
	CNtlTSGroup
	{
		gid = 12;

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
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckDistWithNPC
			{
				dist = "30.000000";
				idxlist = "4191121;";
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
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckNPCDead
			{
				idxlist = "4191121;";
			}
		}
	}
}

