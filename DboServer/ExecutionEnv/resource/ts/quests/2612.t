CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2612;
	title = 261202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;4;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "5101202;";
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
				tblidx = 261220;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickObject
			{
				objidx = "1291;";
				widx = 1;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 261207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5101202;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2613;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 5;
			cid = 100;
			rwdzeny = 0;
			desc = 261214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 261201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 261209;
				ctype = 1;
				idx = 5101202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 261208;
				area = 261201;
				goal = 261204;
				m0fz = "-3701.899902";
				m0widx = 1;
				scitem = -1;
				sort = 261205;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "-505.570007";
				m0ttip = 261215;
				rwd = 100;
				taid = 1;
				title = 261202;
				gtype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 261208;
			gtype = 2;
			oklnk = 2;
			area = 261201;
			goal = 261204;
			sort = 261205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 261202;
		}
	}
}

