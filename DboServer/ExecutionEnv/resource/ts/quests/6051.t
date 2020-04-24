CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6051;
	title = 605102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 6051;
				start = 0;
				taid = 1;
				uof = 1;
			}
		}
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
				eitype = 1;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 605109;
				ctype = 1;
				idx = 4151108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 605108;
				area = 605101;
				goal = 605104;
				m0fz = "-1633.979004";
				m0widx = 1;
				scitem = -1;
				sort = 605105;
				stype = 8;
				m0fy = "-37.009998";
				m0pat = 2;
				grade = -1;
				m0fx = "5090.109863";
				m0ttip = 605115;
				rwd = 100;
				taid = 1;
				title = 605102;
				gtype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1213;
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
			cont = 605108;
			gtype = 1;
			oklnk = 2;
			area = 605101;
			goal = 605104;
			sort = 605105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 605102;
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
			stdiag = 605107;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4151108;";
			}
			CDboTSCheckClrQst
			{
				and = "6050;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 605114;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 605101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1213;
			}
			CDboTSClickNPC
			{
				npcidx = "4151108;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckSToCEvt
			{
				itype = 1;
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

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1213;
				taid = 1;
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
			stdiag = 605107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

