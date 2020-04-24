CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6041;
	title = 604102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 253;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
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
				itype = 3;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 604109;
				ctype = 1;
				idx = 4751407;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 604108;
				area = 604101;
				goal = 604104;
				m0fz = "3728.517090";
				m0widx = 1;
				scitem = -1;
				sort = 604105;
				stype = 8;
				m0fy = "-56.708000";
				m0pat = 2;
				grade = -1;
				m0fx = "6194.335938";
				m0ttip = 604115;
				rwd = 100;
				taid = 1;
				title = 604102;
				gtype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 1206;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 604108;
			gtype = 1;
			oklnk = 2;
			area = 604101;
			goal = 604104;
			sort = 604105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 604102;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "6;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 604107;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4751407;";
			}
			CDboTSCheckClrQst
			{
				and = "6040;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 604114;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 604101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1206;
			}
			CDboTSClickNPC
			{
				npcidx = "4751407;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 604107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1206;
				taid = 1;
				type = 1;
			}
		}
	}
}

