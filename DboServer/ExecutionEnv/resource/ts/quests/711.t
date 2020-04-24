CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 0;
	tid = 711;
	title = 71102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 711;
				start = 0;
				taid = 2;
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
				sid = 711;
				start = 1;
				taid = 2;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 58;
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
				conv = 71109;
				ctype = 1;
				idx = 4754201;
				taid = 2;
			}
			CDboTSActTWaitTS
			{
				taid = 3;
				time = 3000;
			}
			CDboTSActRegQInfo
			{
				cont = 71108;
				gtype = 3;
				area = 71101;
				goal = 71104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 71105;
				stype = 2;
				taid = 1;
				title = 71102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 71107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 7;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 23;
			}
			CDboTSClickNPC
			{
				npcidx = "4754201;";
			}
			CDboTSCheckClrQst
			{
				and = "682;";
				flink = 1;
				flinknextqid = "712;";
				not = 0;
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 71108;
			gtype = 3;
			oklnk = 2;
			area = 71101;
			goal = 71104;
			sort = 71105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 71102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 71114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 71101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5061106;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;2;";
		}
	}
}

