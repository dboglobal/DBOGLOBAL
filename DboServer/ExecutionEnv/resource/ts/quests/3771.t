CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3771;
	title = 377102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 377107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "1654115;";
			}
			CDboTSCheckClrQst
			{
				and = "3770;";
				flink = 1;
				not = 0;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 3771;
				start = 0;
				taid = 2;
				uof = 0;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1630;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 3771;
				start = 1;
				taid = 1;
				uof = 1;
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
				conv = 377109;
				ctype = 1;
				idx = 1654115;
				taid = 2;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1630;
				taid = 3;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 377108;
				area = 377101;
				goal = 377104;
				m0fz = "-6630.330078";
				m0widx = 15;
				scitem = -1;
				sort = 377105;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "-3757.449951";
				m0ttip = 377115;
				rwd = 100;
				taid = 1;
				title = 377102;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 377114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 377101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1654115;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 377108;
			gtype = 1;
			oklnk = 2;
			area = 377101;
			goal = 377104;
			sort = 377105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 377102;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 1630;
			}
			CDboTSColRgn
			{
				widx = 15;
				cct = 1;
				crt = 0;
				r = "20.000000";
				x = "-3757.449951";
				z = "-6630.129883";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 377120;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 377107;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1630;
				taid = 1;
				type = 1;
			}
		}
	}
}

