CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3785;
	title = 378502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1591;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 378507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 69;
			}
			CDboTSClickNPC
			{
				npcidx = "3471301;";
			}
			CDboTSCheckClrQst
			{
				and = "3782;";
				flink = 0;
				flinknextqid = "3849;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 378509;
				ctype = 1;
				idx = 3471301;
				taid = 1;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1591;
				taid = 3;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 378508;
				area = 378501;
				goal = 378504;
				m0fz = "-5617.000000";
				m0widx = 15;
				scitem = -1;
				sort = 378505;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = -1;
				m0fx = "-4655.000000";
				m0ttip = 378515;
				rwd = 100;
				taid = 2;
				title = 378502;
				gtype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 378508;
			gtype = 1;
			oklnk = 2;
			area = 378501;
			goal = 378504;
			sort = 378505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 378502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 378514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 378501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1791102;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 378507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1591;
				taid = 1;
				type = 1;
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

