CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3762;
	title = 376202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 376209;
				ctype = 1;
				idx = 3451101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 376208;
				area = 376201;
				goal = 376204;
				m0fz = "-2653.159912";
				m0widx = 15;
				scitem = -1;
				sort = 376205;
				stype = 2;
				m0fy = "-79.690002";
				m0pat = 0;
				grade = 132203;
				m0fx = "2121.689941";
				m0ttip = 376215;
				rwd = 100;
				taid = 1;
				title = 376202;
				gtype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 376207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3451101;";
			}
			CDboTSCheckClrQst
			{
				and = "3757;";
				flink = 0;
				flinknextqid = "3763;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
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
			cont = 376208;
			gtype = 1;
			oklnk = 2;
			area = 376201;
			goal = 376204;
			sort = 376205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 376202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 376214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 376201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3183301;";
			}
		}
	}
}

