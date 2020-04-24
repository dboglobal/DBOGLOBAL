CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2924;
	title = 292402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 292407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSCheckClrQst
			{
				and = "2923;";
				flink = 0;
				flinknextqid = "2929;";
				not = 0;
			}
			CDboTSClickObject
			{
				objidx = "2121;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99161;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 5;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99161;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 3;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 99161;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "5;4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 696;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 292408;
				area = 292401;
				goal = 292404;
				m0fz = "-3609.949951";
				m0widx = 1;
				scitem = -1;
				sort = 292405;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "2133.060059";
				m0ttip = 292415;
				rwd = 100;
				taid = 1;
				title = 292402;
				gtype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 292408;
			gtype = 2;
			oklnk = 2;
			area = 292401;
			goal = 292404;
			sort = 292405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 292402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 292414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 292401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751314;";
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
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 292407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
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

			CDboTSActItem
			{
				iidx0 = 99161;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
	}
}

