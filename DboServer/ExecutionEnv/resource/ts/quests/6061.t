CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6061;
	title = 606102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 8;
			prelnk = "7;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 430;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "2;";

			CDboTSActItem
			{
				iidx0 = 99096;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1214;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "100;";
			nolnk = 5;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1214;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 606107;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4261111;";
			}
			CDboTSCheckClrQst
			{
				and = "6060;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 5;
			prelnk = "4;3;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "0;1;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 606114;
			nextlnk = 3;
			rwdexp = 0;
			rwdtbl = 606101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "8;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1214;
			}
			CDboTSClickNPC
			{
				npcidx = "4261111;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 606108;
			gtype = 1;
			oklnk = 2;
			area = 606101;
			goal = 606104;
			sort = 606105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 606102;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 7;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 606109;
				ctype = 1;
				idx = 4261111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 606108;
				area = 606101;
				goal = 606104;
				m0fz = "-2401.800049";
				m0widx = 1;
				scitem = 99096;
				sort = 606105;
				stype = 8;
				m0fy = "-40.393002";
				m0pat = 2;
				grade = -1;
				m0fx = "4475.160156";
				m0ttip = 606115;
				rwd = 100;
				taid = 1;
				title = 606102;
				gtype = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99096;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1214;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 606107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

