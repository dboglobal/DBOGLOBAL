CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3004;
	title = 300402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 300407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "3032108;";
			}
			CDboTSCheckClrQst
			{
				and = "2996;";
				flink = 0;
				flinknextqid = "3005;3008;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
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
				itype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 300414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 300401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3032108;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 300408;
			gtype = 2;
			oklnk = 2;
			area = 300401;
			goal = 300404;
			sort = 300405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 300402;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 300409;
				ctype = 1;
				idx = 3032108;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 12914300;
				cnt0 = 15;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 300408;
				m1fx = "-326.489990";
				m1fy = "0.000000";
				sort = 300405;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-668.109985";
				grade = 132203;
				m0fx = "-121.989998";
				m0ttip = 300415;
				m1pat = 2;
				m1widx = 6;
				rwd = 100;
				taid = 1;
				title = 300402;
				gtype = 2;
				area = 300401;
				goal = 300404;
				m0fz = "-692.159973";
				m0widx = 6;
				m1ttip = 300416;
				scitem = -1;
				stype = 1;
			}
		}
	}
}

