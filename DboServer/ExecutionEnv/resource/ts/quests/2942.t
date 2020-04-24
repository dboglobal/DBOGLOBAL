CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2942;
	title = 294202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 294207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSClickNPC
			{
				npcidx = "4752110;";
			}
			CDboTSCheckClrQst
			{
				and = "2936;";
				flink = 0;
				flinknextqid = "2943;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 294214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 294201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511612;";
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
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 294208;
			gtype = 1;
			oklnk = 2;
			area = 294201;
			goal = 294204;
			sort = 294205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 294202;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 294209;
				ctype = 1;
				idx = 4752110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 294208;
				area = 294201;
				goal = 294204;
				m0fz = "-3854.120117";
				m0widx = 1;
				scitem = -1;
				sort = 294205;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "3159.719971";
				m0ttip = 294215;
				rwd = 100;
				taid = 1;
				title = 294202;
				gtype = 1;
			}
		}
	}
}

