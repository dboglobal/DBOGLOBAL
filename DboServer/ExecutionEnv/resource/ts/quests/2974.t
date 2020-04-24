CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2974;
	title = 297402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 297409;
				ctype = 1;
				idx = 4531104;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 2974;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = 297415;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 297408;
				area = 297401;
				goal = 297404;
				m0fz = "-4595.910156";
				m0widx = 1;
				scitem = -1;
				sort = 297405;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				grade = -1;
				m0fx = "3473.489990";
				m0ttip = 297415;
				rwd = 100;
				taid = 1;
				title = 297402;
				gtype = 3;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 297414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 297401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4531104;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 297407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSCheckClrQst
			{
				and = "2973;";
				flink = 0;
				flinknextqid = "2969;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4531104;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 297408;
			gtype = 3;
			oklnk = 2;
			area = 297401;
			goal = 297404;
			sort = 297405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 297402;
		}
	}
}

